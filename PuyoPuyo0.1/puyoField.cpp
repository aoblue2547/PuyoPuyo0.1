#include "puyoField.hpp"
#include "core.hpp"
#include "frames.hpp"
#include "Stage.hpp"
#include "MovePuyo.hpp"

puyoField::puyoField(int h, int w) :grid(h, Array<Puyo>(w, Puyo::Wall)), width(w), height(h) {}

Array<Puyo>& puyoField::operator[](size_t h) {
	return grid[h];
}
const Array<Puyo>& puyoField::operator[](size_t h) const {
	return grid[h];
}

void puyoField::init() {
	grid = Array<Array<Puyo>>(height, Array<Puyo>(width, Puyo::Wall));
}

void puyoField::draw(bool isTwoPlayer) {
	const int siz = Stage::blockSize;
	const int offset = siz + Stage::blockOffset;
	PuyoChip chip;
	for (auto y : step(height)) {
		for (auto x : step(width)) {
			//左下から描画するための変数
			const int Y = Stage::height - y - 1;
			if (grid[Y][x] < Puyo::Ojama) {
				//同じ色のぷよが接続しているか判定
				//1<<0:上
				//1<<1:下
				//1<<2:左
				//1<<3:右
				//+フラグ*6をする
				//for文で書こうと思ったけど、方向がdircと違うからやめた
				int flag = 0;
				if (Y + 1 < Stage::height and grid[Y][x] == grid[Y + 1][x]) {
					flag |= 1 << 0;
				}
				if (Y - 1 >= 0 and grid[Y][x] == grid[Y - 1][x]) {
					flag |= 1 << 1;
				}
				if (x - 1 >= 0 and grid[Y][x] == grid[Y][x - 1]) {
					flag |= 1 << 2;
				}
				if (x + 1 < Stage::width and grid[Y][x] == grid[Y][x + 1]) {
					flag |= 1 << 3;
				}
				chip.get(grid[Y][x] + flag * 6).draw(Stage::X_o(isTwoPlayer) + x * offset, y * offset + Stage::Y_o());
			}
			else {
				if (grid[Y][x] == Puyo::Ojama) {
					chip.get(5 + 6 * 2).draw(Stage::X_o(isTwoPlayer) + x * offset, y * offset + Stage::Y_o());
				}
			}
		}
	}
}

int puyoField::append(MovePuyo puyo) {
	const int x = puyo.getx();
	const int y = (int)puyo.gety();//切り捨て？
	grid[y][x] = puyo.getOya();
	const int cx = x + dirc[puyo.getDirection()][0];
	const int cy = y + dirc[puyo.getDirection()][1];
	grid[cy][cx] = puyo.getKo();
	return frames::set;
}

int puyoField::tigiriDrop() {
	//浮いているぷよを探す(ちぎりなので一つだけ)
	int x = 0, y = 0;
	for (int iy = 1; iy < Stage::height; ++iy) {
		for (int ix = 0; ix < Stage::width; ++ix) {
			if (grid[iy][ix] >= Puyo::Wall)continue;
			if (grid[iy - 1][ix] < Puyo::Wall)continue;
			//自分の一つ下にぷよが無かったら
			x = ix;
			y = iy;
			break;
		}
		if (y > 0)break;
	}
	
	if (y == 0)return 0;

	//浮いているぷよの段差を計算する
	int d = 0;
	for (int iy = y - 1; iy >= 0; --iy) {
		if (grid[iy][x] >= Puyo::Wall)++d;
		else {
			break;
		}
	}

	grid[y - d][x] = grid[y][x];
	grid[y][x] = Puyo::Back;

	return d;

}

//連鎖のスコアを返す、ぷよの場所は消去するが下に落とすのは別にする
int puyoField::rensaScore(int chain) {
	//4個以上繋がっているところを見つける
	const int w = Stage::width, h = Stage::height;
	DisjointSet<int> uf((size_t)h * w);

	//UnionFindでくっつける
	for (int i = 0; i < h * w; ++i) {
		const int x = i % w, y = i / w;
		if (grid[y][x] >= Puyo::Ojama)continue;
		//4方向探索
		for (int j = 0; j < 4; ++j) {
			int cx = x + dirc[j][0], cy = y + dirc[j][1];
			if (cx < 0 or cy < 0 or cx >= w or cy >= h)continue;
			if (grid[y][x] == grid[cy][cx]) {
				uf.merge(i, cy * w + cx);
			}
		}
	}

	//4個以上くっついてるぷよの場所を持つ配列
	Array<int> place;
	//そのうちUnionFindで親である頂点を持つ配列
	Array<int> par;
	//消えたぷよの色をもつ
	Array<bool> eraseColor(5, false);
	//何連結が何個あるかの配列(4,5,6,...,11以上)
	Array<int> renketu(8);
	//消えたぷよの個数を数える
	int puyocnt = 0;
	for (int i = 0; i < h * w; ++i) {
		const int x = i % w, y = i / w;
		int siz = uf.size(i);
		if (siz >= 4) {
			puyocnt++;
			place.push_back(i);
			//周りにおじゃまぷよがあればそれも入れる
			for (int j = 0; j < 4; ++j) {
				const int cx = x + dirc[j][0], cy = y + dirc[j][1];
				if (cx < 0 or cy < 0 or cx >= w or cy >= h)continue;
				if (grid[cy][cx] == Puyo::Ojama) {
					place.push_back(cy * w + cx);
				}
			}
			if (uf.find(i) == i) {
				eraseColor[grid[y][x]] = true;
				par.push_back(i);
				const int iter = Min(siz - 4, 7);
				renketu[iter]++;
			}
		}
	}

	//消えてなかったら0を返す
	if (place.size() == 0) {
		return 0;
	}

	//4連結以上のぷよを消す
	for (auto i : place) {
		const int x = i % w, y = i / w;
		grid[y][x] = Puyo::Wall;
	}

	return Score::rensaScore(chain, puyocnt, renketu, eraseColor);
}

//浮いているぷよをすべて消す
int puyoField::allDrop() {
	int d = 0;
	int r = 0;
	while (r = tigiriDrop()) {
		d = Max(d, r);
	}
	return d;
}

bool puyoField::isZenkeshi() {
	int cnt = 0;
	for (auto& v : grid) {
		cnt += std::count_if(v.begin(), v.end(), [](Puyo p) {return p < Puyo::Wall; });
	}
	return cnt == 0;
}

bool puyoField::isGameOver() {
	return grid[12][2] < Puyo::Wall;
}
