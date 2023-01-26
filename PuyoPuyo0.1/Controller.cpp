#include "Controller.hpp"
#include "core.hpp"
#include "frames.hpp"
#include "Stage.hpp"
#include "puyoField.hpp"
#include "MovePuyo.hpp"
#include "GameScore.hpp"

Controller::Controller(playeris p) :button(5), pro(0) {
	setPlayer(p);
}

bool Controller::operator[](int i) {
	return button[i].pressed();
}

void Controller::setKey(int i, int b) {
	button[i].update(b);
}

//コントローラーの入力を更新する
void Controller::update(int f) {
	deltaFrame = f;
	setKey(3, rightTurn.pressed());
	setKey(4, leftTurn.pressed());
	if (player == playeris::keyboard) {
		setKey(0, rightMove.pressed());
		setKey(1, leftMove.pressed());
		setKey(2, downMove.pressed());
	}
	if (player == playeris::procon) {
		auto [x, y] = pro.LStick();
		setKey(0, (x >= 0.3) | rightMove.pressed());
		setKey(1, (x <= -0.3) | leftMove.pressed());
		setKey(2, (y >= 0.3) | downMove.pressed());
	}
}

void Controller::setPlayer(playeris p) {
	if (p == player)return;
	player = p;
	Print << pro.isConnected();
	switch (p) {
	case keyboard:
		rightMove = KeyRight | KeyD;
		leftMove = KeyLeft | KeyA;
		downMove = KeyDown | KeyS;
		rightTurn = KeyX | KeyM;
		leftTurn = KeyZ | KeyN;
		break;
	case procon:
		rightMove = pro.povRight;
		leftMove = pro.povLeft;
		downMove = pro.povDown;
		rightTurn = pro.buttonA;
		leftTurn = pro.buttonB;
		break;
	case AI:
		break;
	default:
		break;
	}
}

int Controller::getDeltaFrame() const {
	return deltaFrame;
}

//コントローラ入力からぷよの座標を更新する
void Controller::controll(MovePuyo& puyo, puyoField& grid, GameScore& score) {
	//yをdoubleにするかintにするかどうか
	const double y = puyo.gety();
	const int x = puyo.getx();
	const int d = puyo.getDirection();
	//子ぷよの座標
	const int cx = x + dirc[d][0];
	const int cy = y + dirc[d][1];
	//横移動の、最初止まって、そのあとは止まらずに移動するやつを再現する用
	//右ボタンが押された
	if (button[0].down() or frames::toFrames(button[0].pressedDuration()) >= 12
		and x + 1 < Stage::width and cx + 1 < Stage::width
		and grid[y][x + 1] >= Puyo::Wall and grid[cy][cx + 1] >= Puyo::Wall)
	{
		puyo.moveRight(button[0].down());
	}
	//左ボタンが押された
	else if (button[1].down() or frames::toFrames(button[1].pressedDuration()) >= 12
		and x - 1 >= 0 and cx - 1 >= 0
		and grid[y][x - 1] >= Puyo::Wall and grid[cy][cx - 1] >= Puyo::Wall)
	{
		puyo.moveLeft(button[1].down());
	}
	//下ボタンが押された
	else if (button[2].pressed()) {
		int ground = 0;
		//子ぷよの判定もする
		if (y >= 1 and grid[(int)y - 1][x] < Puyo::Wall) {
			ground = (int)y;
		}
		if (cy - 1 >= 0 and grid[cy - 1][cx] < Puyo::Wall) {
			ground = Max(ground, cy);
		}
		if (d == 2) {
			++ground;
		}
		//quickFall()はこれ以上ぷよを下げられなかったときにtrueを返す
		if (puyo.quickFall(ground)) {
			//TODO:freefallframeとfixedframeの兼ね合いがどうなるか検証
			puyo.plusFreeFallFrame();
		}
		else {
			puyo.resetFreeFallFrame();
			plusScoreFrame += deltaFrame;
			if (plusScoreFrame >= 2) {
				score += 1;
				plusScoreFrame -= 2;
			}
		}
	}
	//Aボタンが押された(時計回りに回転)
	if (button[3].down()) {
		const int newd = (d + 1) % 4;
		const int newcx = x + dirc[newd][0];
		const double newcy = y + dirc[newd][1];
		int nx = x + dirc[(newd + 2) % 4][0];
		double ny = y + dirc[(newd + 2) % 4][1];
		int nncx = nx + dirc[newd][0];
		double nncy = ny + dirc[newd][1];
		//子ぷよの位置はvalidか
		if (newcy >= 0 and newcx >= 0 and newcx < Stage::width
			and grid[newcy][newcx] >= Puyo::Wall)
		{
			puyo.setDirction(newd);
			quickTurn = false;
		}
		//親ぷよの位置を移動させることで子ぷよが移動できたら移動させる
		else if (nncy >= 0 and nncx >= 0 and nncx < Stage::width
			and grid[nncy][nncx] >= Puyo::Wall
			and ny >= 0 and nx >= 0 and nx < Stage::width
			and grid[ny][nx] >= Puyo::Wall)
		{
			//親ぷよの位置も移動させる
			puyo.setxy(nx, ny);
			puyo.setDirction(newd);
			quickTurn = false;
		}

		else if (quickTurn) {
			quickTurn = false;
			puyo.setxy(cx, cy);
			puyo.setDirction(d + 2);
		}
		else {
			quickTurn = true;
		}

	}
	//Bボタンが押された(反時計回りに回転)
	else if (button[4].down()) {
		int newd = (d + 3) % 4;
		int newcx = x + dirc[newd][0];
		double newcy = y + dirc[newd][1];
		int nx = x + dirc[(newd + 2) % 4][0];
		double ny = y + dirc[(newd + 2) % 4][1];
		int nncx = nx + dirc[newd][0];
		double nncy = ny + dirc[newd][1];
		//子ぷよの位置ははvalidか
		if (newcy >= 0 and newcx >= 0 and newcx < Stage::width
			and grid[newcy][newcx] >= Puyo::Wall)
		{
			puyo.setDirction(newd);
			quickTurn = false;
		}
		//親ぷよの位置を移動させることで子ぷよが移動できたら移動させる
		else if (nncy >= 0 and nncx >= 0 and nncx < Stage::width
			and grid[nncy][nncx] >= Puyo::Wall
			and ny >= 0 and nx >= 0 and nx < Stage::width
			and grid[ny][nx] >= Puyo::Wall)
		{
			//親ぷよの位置も移動させる
			puyo.setxy(nx, ny);
			puyo.setDirction(newd);
			quickTurn = false;
		}

		else if (quickTurn) {
			quickTurn = false;
			puyo.setxy(cx, cy);
			puyo.setDirction(d + 2);
		}
		else {
			quickTurn = true;
		}
	}
}
