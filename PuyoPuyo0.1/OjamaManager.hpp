#pragma once

class puyoField;

//おじゃま予告の描画と、おじゃまを降らせる
class OjamaManager {
	//自分のおじゃま予告のおじゃまぷよの個数
	int myOjama = 0;
	//自分の陣地に降ることが確定したおじゃまぷよの個数
	int setOjama = 0;
	//相手に送るおじゃまぷよの個数
	int sendOjama = 0;
public:
	void init();
	//自分の連鎖で発生したおじゃまぷよの個数を入れる
	void update(int cnt);
	int send();
	//相手の連鎖で発生したおじゃまぷよを受け取る
	void receive(int cnt);
	//現在予告にあるおじゃまぷよが降ることを確定させる
	void set();
	//おじゃまを降らせる
	int fallOjama(puyoField& grid);
	void drawYokoku(bool isTwoPlayer);
};
