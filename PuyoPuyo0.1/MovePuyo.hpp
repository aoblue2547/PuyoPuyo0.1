#pragma once

enum Puyo;
class puyoField;

//動かすぷよのクラス
class MovePuyo {
	Puyo oya;
	Puyo ko;
	int x;
	double y;//0.5単位の座標,左下から0,0
	int direction = 0;//子ぷよの方向,0から順に上、右、下、左
	int freefallframe = 0;//累積フレーム,自由落下のときに使う
	int deltaframe = 0;
	bool isOnGround = false;
	//地面についてから設置するまでのフレーム
	int fixedFrame = 0;
	bool isset = false;
public:
	MovePuyo(Puyo o, Puyo k, int X, int Y);
	int getx();
	double gety();
	void setxy(int newX, int newY);
	int getDirection();
	Puyo getOya();
	Puyo getKo();
	bool isSet();
	bool OnGround();
	void update(const int f, const puyoField& grid);
	void resetFreeFallFrame();
	void plusFreeFallFrame();
	void plusFixedFrame();
	void setDirction(int d);
	void moveRight(bool clicked);
	void moveLeft(bool clicked);
	bool quickFall(int ground);
	void draw(bool isTwoPlayer = false);
};
