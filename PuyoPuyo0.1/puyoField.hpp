#pragma once

class MovePuyo;
enum Puyo;

//ぷよの場所など
class puyoField {
	const int width, height;
	Array<Array<Puyo>> grid;
public:
	puyoField(int h, int w);
	Array<Puyo>& operator[](size_t h);
	const Array<Puyo>& operator[](size_t h) const;
	void init();
	void draw(bool isTwoPlayer = false);
	int append(MovePuyo puyo);
	int tigiriDrop();
	int rensaScore(int chain);
	int allDrop();
	bool isZenkeshi();
	bool isGameOver();
};
