#pragma once

class MovePuyo;

//ツモの生成
class Haipuyo {
	size_t siz = 65536;
	int tumoNumber = 0;//使用するツモの番号
	int current = 0;//ツモを何番目まで読んでいるか
	Array<String> tumo;
public:
	Haipuyo();
	Haipuyo& operator=(Haipuyo& hai);
	void reset();
	void random();
	void setTumoNumber(int num);
	int getTumoNumber();
	MovePuyo next();
	void drawNext(bool isTwoPlayer = false);
};
