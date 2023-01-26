#pragma once
#include <bitset>

class boolKey;
class MovePuyo;
class puyoField;
class GameScore;
enum playeris;

//ボタン操作を持つクラス
class Controller {
private:
	//操作するのが人間かAIか
	playeris player;
	//右、左、下、A,Bの順
	Array<boolKey> button;
	int deltaFrame = 0;
	//下移動のスコア加算用
	int plusScoreFrame = 0;
	//クイックターン用の変数
	bool quickTurn = false;
private:
	InputGroup rightTurn;
	InputGroup leftTurn;
	InputGroup rightMove;
	InputGroup leftMove;
	InputGroup downMove;
	ProController pro;
private:
	void setKey(int i, int b);
public:
	Controller(playeris p);
	bool operator[](int i);
	void update(int f);
	void setPlayer(playeris p);
	int getDeltaFrame() const;
	void controll(MovePuyo& puyo, puyoField& grid, GameScore& score);

};
