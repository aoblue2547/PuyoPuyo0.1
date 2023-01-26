#include "OjamaManager.hpp"
#include "core.hpp"
#include "Stage.hpp"
#include "frames.hpp"
#include "puyoField.hpp"

void OjamaManager::init() {
	myOjama = 0;
	setOjama = 0;
	sendOjama = 0;
}

void OjamaManager::update(int cnt) {
	myOjama -= cnt;
	if (myOjama < 0) {
		sendOjama = -myOjama;
		myOjama = 0;
	}
}
int OjamaManager::send() {
	int res = sendOjama;
	sendOjama = 0;
	return res;
}
	//相手の連鎖で発生したおじゃまぷよを受け取る
void OjamaManager::receive(int cnt) {
	myOjama += cnt;
}
	//現在予告にあるおじゃまぷよが降ることを確定させる
void OjamaManager::set() {
	setOjama += myOjama;
	myOjama = 0;
}
	//おじゃまを降らせる
int OjamaManager::fallOjama(puyoField& grid) {
	const int w = Stage::width, h = Stage::height;
	//おじゃまは最大で5段(30個)しか一度に降らない
	int fallojama = 0;
	if (setOjama >= 30) {
		fallojama = 30;
		setOjama -= 30;
	}
	else {
		fallojama = setOjama;
		setOjama = 0;
	}
	int frame = 0;
	int oframe = frames::ojamaCnt(fallojama);
	Array<int> oja(w, fallojama / w);//各列に降らせるおじゃまの量

	fallojama %= w;
	//ぷよぷよ通は端数はランダム
	Array<bool> pl(w, false);
	while (fallojama--) {
		int i;
		do {
			i = Random<int>(0, w - 1);
		} while (pl[i]);
		pl[i] = true;
	}
	for (int i : step(w)) {
		oja[i] += pl[i];
	}
	//実際に各列におじゃまを降らせる
	for (int x : step(w)) {
		int start = 0;
		for (; start < h; ++start) {
			if (grid[start][x] >= Puyo::Wall) {
				break;
			}
		}
		for (int dy : step(oja[x])) {
			if (start + dy >= h - 1)break;
			grid[start + dy][x] = Puyo::Ojama;
			frame = Max(frame, frames::ojamaPosition[(12 - (start + dy)) * w + x]);
		}
	}
	return frame + oframe;
}

//描画する予告おじゃまぷよ6つまでを決める
void OjamaManager::drawYokoku(bool isTwoPlayer) {
	int ojama = myOjama + setOjama;
	//予告おじゃまぷよのサイズ
	constexpr std::array<int, 6> yokoku = { 1,6,30,180,360,720 };
	std::array<int, 6> iter = { -1 };
	int ojamaSize = 5;
	for (int j : step(6)) {
		while (ojamaSize >= 0 and ojama - yokoku[ojamaSize] < 0) {
			--ojamaSize;
		}
		if (ojamaSize < 0)iter[j] = -1;
		else {
			iter[j] = ojamaSize;
			ojama -= yokoku[ojamaSize];
		}
	}
	for (int i = 0; i < 6; ++i) {
		if (iter[i] < 0)break;
		TextureAsset(U"ojama")(iter[i] * Stage::blockSize, 0, Stage::blockSize).draw(Stage::X_o(isTwoPlayer) + i * Stage::blockSize, Stage::Y_o());
	}
}
