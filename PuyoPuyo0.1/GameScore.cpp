#include "GameScore.hpp"
#include "Stage.hpp"

GameScore::GameScore() :score(0), currentScore(0), font{ 30 } {}

GameScore& GameScore::operator +=(int n) {
	score += n;
	currentScore += n;
	return *this;
}

void GameScore::init() {
	score = 0;
	currentScore = 0;
}

int GameScore::calculateOjama() {
	constexpr int rate = 70;
	int res = currentScore / 70;
	currentScore -= res * rate;
	return res;
}

void GameScore::draw(bool isTwoPlayer) {
	const int x = Stage::X_o(isTwoPlayer) + Stage::lengthW / 2;
	const int y = Stage::Y_o() + Stage::lengthH + 30;
	font(score).drawAt(x, y, Palette::Black);
}

