#pragma once
#include "frames.hpp"
#include "Stage.hpp"
#include "MovePuyo.hpp"
#include "puyoField.hpp"
#include "Haipuyo.hpp"
#include "GameScore.hpp"
#include "Controller.hpp"
#include "OjamaManager.hpp"

/*
ぷよぷよの1ゲームを管轄するクラス
mainループ上でrunLoop()を呼ぶと状態が更新される
*/


class GameManager {
private://ゲームを進行するための変数
	double accumulatesec;
	Array<puyoField> Grid;
	//ゲームで使う配ぷよ
	Array<Haipuyo> haipuyo;
	//自分が動かすぷよ
	Array<MovePuyo> mPuyo;
	//操作を受け付ける
	Array<Controller> controllers;
	//スコア
	Array<GameScore> Score;
	//予告おじゃまぷよの管理
	Array<OjamaManager> OjamaMana;
	//これがなくなるまで操作を受け付けない、ちぎりや連鎖時などに増える
	Array<int> FreezeFrame;
	//現在の連鎖数
	Array<int> cntRensa;

private://ゲームの状態を管理するための変数
	bool isPlaying = false;
	

private://通信関係
	//1pと2pのAIのファイルパス
	Array<String> AIpath;
	std::array<ChildProcess, 2> aiConnector;

private:
	void draw();
	void update();

public:
	GameManager(playeris p1, playeris p2);
	void resume();
	void stop();
	bool nowPlaying();
	void reset();
	void runLoop();
	int tumoNumber();
	void useTumo(int);
	void setPlayer(int, playeris);

};
