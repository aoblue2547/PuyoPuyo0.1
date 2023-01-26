#include "MovePuyo.hpp"
#include "core.hpp"
#include "frames.hpp"
#include "Stage.hpp"
#include "puyoField.hpp"

MovePuyo::MovePuyo(Puyo o, Puyo k, int X, int Y) :oya(o), ko(k), x(X), y(Y) {}

int MovePuyo::getx() {
	return x;
}

double MovePuyo::gety() {
	return y;
}

void MovePuyo::setxy(int newX, int newY) {
	x = newX;
	y = newY;
}

int MovePuyo::getDirection() {
	return direction;
}

Puyo MovePuyo::getOya() {
	return oya;
}

Puyo MovePuyo::getKo() {
	return ko;
}

bool MovePuyo::isSet() {
	return isset;
}

bool MovePuyo::OnGround() {
	return isOnGround;
}

void MovePuyo::update(const int f, const puyoField& grid) {
	deltaframe = f;
	freefallframe += f;
	isOnGround = false;
	//yの小数点以下を抜き出す
	double yd = y - floor(y);
	//親ぷよと子ぷよでそれぞれ接地判定をする
	if (y == 0 or (y == 1 and direction == 2)) {
		isOnGround = true;
	}
	else if (yd == 0) {
		if (grid[y - 1][x] < Puyo::Wall or grid[y + dirc[direction][1] - 1][x + dirc[direction][0]] < Puyo::Wall) {
			isOnGround = true;
		}
	}

	//接地していたなら
	if (isOnGround) {
		fixedFrame += f;
		if (fixedFrame >= frames::fixed) {
			isset = true;
		}
	}
	//resetFreeFallFrame()が呼ばれていない、つまり何も操作しないままなら自由落下させる
	else if (freefallframe >= frames::freeFall) {
		freefallframe -= frames::freeFall;
		if (yd >= 0.5) {
			y += 0.5;
			y -= y - floor(y) + 1;
		}
		else if (yd > 0) {
			y = y - yd;
		}
		else {
			y = y - 0.5;
		}
	}
	
}

void MovePuyo::resetFreeFallFrame() {
	freefallframe = 0;
}

void MovePuyo::plusFreeFallFrame() {
	freefallframe += 1;
}

void MovePuyo::plusFixedFrame() {
	fixedFrame += deltaframe * 2;
}

void MovePuyo::setDirction(int d) {
	direction = d % 4;
}

void MovePuyo::moveRight(bool clicked = false) {
	if (clicked) {
		x += 1;
	}
	else x += deltaframe;
}

void MovePuyo::moveLeft(bool clicked = false) {
	if (clicked) {
		x -= 1;
	}
	else x -= deltaframe;
}

bool MovePuyo::quickFall(int ground) {
	const double speed = 0.43;
	const double fallDistance = speed * deltaframe;
	if (y - fallDistance > ground) {
		y -= fallDistance;
		return false;
	}
	//これ以上下がらないときはtrueを返す
	else {
		y = ground;
		return true;
	}
}

void MovePuyo::draw(bool isTwoPlayer) {
	const int siz = Stage::blockSize;
	const int offset = siz + Stage::blockOffset;
	const int centx = Stage::X_o(isTwoPlayer) + x * offset + siz / 2;
	const double centy = Stage::Y_o() + (Stage::height - y) * offset - siz / 2;
	PuyoChip chip;
	//親ぷよの描画
	chip.get(oya).drawAt(centx, centy);
	//親ぷよの周りを点滅させる
	if (freefallframe <= frames::freeFall / 2) {
		Circle(centx, centy, siz / 2).drawFrame(0, 2);
	}
	//子ぷよの描画
	const int cx = Stage::X_o(isTwoPlayer) + (x + dirc[direction][0]) * offset + siz / 2;
	const double cy = Stage::Y_o() + (Stage::height - (y + dirc[direction][1])) * offset - siz / 2;
	chip.get(ko).drawAt(cx, cy);
}

