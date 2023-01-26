#include "Haipuyo.hpp"
#include "core.hpp"
#include "Stage.hpp"
#include "MovePuyo.hpp"

PuyoChip chip;

Haipuyo::Haipuyo() {
	TextReader reader{ U"haipuyo.txt" };
	if (not reader) {
		throw Error{ U"Failed to open haipuyo.txt." };
	}
	String line;
	while (reader.readLine(line)) {
		tumo.push_back(line);
	}
}

//相手の配ぷよと同期をとる
Haipuyo& Haipuyo::operator=(Haipuyo& hai) {
	this->setTumoNumber(hai.getTumoNumber());
	return *this;
}

void Haipuyo::reset() {
	current = 0;
}

void Haipuyo::random() {
	tumoNumber = Random<int>(0, 65536 - 1);
}

void Haipuyo::setTumoNumber(int num) {
	tumoNumber = num;
	current = 0;
}

int Haipuyo::getTumoNumber() {
	return tumoNumber;
}

MovePuyo Haipuyo::next() {
	Puyo oya, ko;
	oya = txtToPuyo((char)tumo[tumoNumber][current]);
	ko = txtToPuyo((char)tumo[tumoNumber][current + 1]);
	current += 2;
	//ツモが一周したら
	if (current == 256) {
		current = 0;
	}
	return MovePuyo(oya, ko, 2, 12);
}

//ネクストとネクネクの描画をする関数
void Haipuyo::drawNext(bool isTwoPlayer) {
	Array<Puyo> n(4);
	int c = current;
	for (int i = 0; i < 4; ++i) {
		n[i] = txtToPuyo((char)tumo[tumoNumber][c]);
		++c;
		if (c == 256) {
			c = 0;
		}
	}

	int xo = Stage::X_o(isTwoPlayer);
	int offset = (isTwoPlayer ? -60 : 220);
	int offset2 = (isTwoPlayer ? -100 : 260);
	int yo = Stage::Y_o() + 40;
	//ネクストの描画
	chip.get(n[0]).draw(xo + offset, yo);
	chip.get(n[1]).draw(xo + offset, yo - Stage::blockSize);
	//ネクネクの描画
	chip.get(n[2]).draw(xo + offset2, yo + 80);
	chip.get(n[3]).draw(xo + offset2, yo + 80 - Stage::blockSize);
}
