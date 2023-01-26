#include "GameManager.hpp"
#include <bitset>
#include "core.hpp"
#include "frames.hpp"
#include "Stage.hpp"
#include "MovePuyo.hpp"
#include "puyoField.hpp"
#include "Haipuyo.hpp"
#include "GameScore.hpp"
#include "Controller.hpp"
#include "OjamaManager.hpp"


GameManager::GameManager(playeris p1,playeris p2)
	:accumulatesec(0), Grid(2, puyoField(Stage::height, Stage::width)), haipuyo(2), Score(2), OjamaMana(2), FreezeFrame(2, 0), cntRensa(2, 0) {
	haipuyo[0].random();
	haipuyo[1] = haipuyo[0];
	mPuyo = { haipuyo[0].next(),haipuyo[1].next() };
	controllers.push_back(Controller(p1));
	controllers.push_back(Controller(p2));
}

void GameManager::resume() {
	isPlaying = true;
}

void GameManager::stop() {
	isPlaying = false;
}

bool GameManager::nowPlaying() {
	return isPlaying;
}

void GameManager::reset() {
	accumulatesec = 0;
	haipuyo[0].reset();
	haipuyo[1] = haipuyo[0];
	for (int i = 0; i < 2; ++i) {
		mPuyo[i] = haipuyo[i].next();
		Grid[i].init();
		Score[i].init();
		cntRensa[i] = 0;
		OjamaMana[i].init();
	}
}

//isplaying = false のとき、描画のみ行う
void GameManager::runLoop() {

	if (isPlaying) {
		update();
	}

	draw();

}

//TODO:GameStateを作ってこの関数内をきれいに書き直す
void GameManager::update() {
	accumulatesec += Scene::DeltaTime();
	const int deltaFrame = frames::timeToFrames(accumulatesec);

	//1Pと2Pそれぞれについて
	for (int i = 0; i < 2; ++i) {
		FreezeFrame[i] = Max(FreezeFrame[i] - deltaFrame, 0);
		if (FreezeFrame[i] != 0)continue;

		controllers[i].update(deltaFrame);
		//連鎖中だったら
		if (cntRensa[i] > 0) {
			int sco = Grid[i].rensaScore(cntRensa[i]);
			if (sco != 0) {
				Score[i] += sco;
				cntRensa[i]++;
				FreezeFrame[i] += frames::rensa[Grid[i].allDrop()];
				OjamaMana[i].update(Score[i].calculateOjama());
				OjamaMana[!i].receive(OjamaMana[i].send());
			}
			else {
				if (Grid[i].isZenkeshi()) {
					Score[i] += 2100;
				}
				cntRensa[i] = 0;
				mPuyo[i] = haipuyo[i].next();
				OjamaMana[i].set();
				OjamaMana[!i].set();
				FreezeFrame[i] += OjamaMana[i].fallOjama(Grid[i]);
			}
		}
		//ゲームオーバーでなかったら
		else if (not Grid[i].isGameOver()) {
			//ボタン操作を反映させる
			controllers[i].controll(mPuyo[i], Grid[i], Score[i]);
			//自然落下、接地、設置判定
			mPuyo[i].update(deltaFrame, Grid[i]);
			//ぷよが接地したら
			if (mPuyo[i].OnGround()) {
				//下ボタンが押されていたら設置を早くする
				if (controllers[i][2]) {
					mPuyo[i].plusFixedFrame();
				}
			}
			//ぷよが設置したら
			if (mPuyo[i].isSet()) {
				FreezeFrame[i] += Grid[i].append(mPuyo[i]);
				FreezeFrame[i] += frames::thigiri[Grid[i].tigiriDrop()];
				int sco = Grid[i].rensaScore(cntRensa[i]);
				//ぷよが消えていた場合
				if (sco != 0) {
					Score[i] += sco;
					cntRensa[i]++;
					FreezeFrame[i] += frames::rensa[Grid[i].allDrop()];
					OjamaMana[i].update(Score[i].calculateOjama());
					OjamaMana[!i].receive(OjamaMana[i].send());
				}
				//ぷよが消えていなかった場合
				else {
					mPuyo[i] = haipuyo[i].next();
					FreezeFrame[i] += OjamaMana[i].fallOjama(Grid[i]);
				}
			}
		}
		//ゲームオーバーの場合
		else {

		}

	}
}

void GameManager::draw() {

	for (int i = 0; i < 2; ++i) {
		Stage::draw(i);
		Grid[i].draw(i);
		if (FreezeFrame[i] == 0)mPuyo[i].draw(i);
		if (cntRensa[i] > 0) {
			FontAsset(U"Rensa")(U"{}れんさ！"_fmt(cntRensa[i])).draw(200, 200, Palette::Black);
		}
		haipuyo[i].drawNext(i);
		OjamaMana[i].drawYokoku(i);
		Score[i].draw(i);
	}

}

int GameManager::tumoNumber() {
	return haipuyo[0].getTumoNumber();
}

void GameManager::useTumo(int number) {
	if (number >= 0 and number < 65536) {
		haipuyo[0].setTumoNumber(number);
		haipuyo[1] = haipuyo[0];
		mPuyo[0] = haipuyo[0].next();
		mPuyo[1] = haipuyo[1].next();
	}
}

void GameManager::setPlayer(int i, playeris p) {
	controllers[i].setPlayer(p);
}
