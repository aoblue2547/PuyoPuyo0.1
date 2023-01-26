#include "frames.hpp"

namespace frames {
	//おじゃまの量によるフレーム硬直(画面が揺れる演出)
	//条件はwebサイトから
	int ojamaCnt(int ojama) {
		if (ojama <= 9)return Max(ojama - 8, 0);
		if (ojama <= 11)return ojama - 5;
		if (ojama <= 13)return ojama - 4;
		if (ojama <= 19)return ojama - 3;
		if (ojama <= 22)return ojama - 2;
		if (ojama <= 26)return ojama - 3;
		if (ojama <= 28)return 24;
		else return 25;
		//最大で30個ずつ降る
	}
	//時間をフレームに変換する、余剰分の時間は残して引き算する
	int timeToFrames(double& time) {
		int res = (int)(time * FPS);//切り捨て
		time -= res / (double)FPS;
		return res;
	}

	int toFrames(const double time) {
		return (int)(time * FPS);
	}

}
