#pragma once

//スコアの更新、描画、おじゃまの計算
class GameScore {
	int score;//全体のスコア、画面下部に表示されるもの
	int currentScore;//おじゃまぷよを送る際に使われるもの
	const Font font;
public:
	GameScore();
	GameScore& operator +=(int n);
	void init();
	int calculateOjama();
	void draw(bool isTwoPlayer = false);
};
