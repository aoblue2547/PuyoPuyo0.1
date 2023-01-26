#pragma once

namespace frames {
	constexpr int FPS = 60;
	//自然落下(0.5ブロック分)にかかるフレーム
	constexpr int freeFall = 16;
	//ぷよの回転時のフレーム
	constexpr int rotate = 10;
	//左右移動時のフレーム(適当)
	//constexpr int move = 5;
	//接地時のフレーム硬直(適当)
	constexpr int set = 9;
	//設置までの予備フレーム(適当)
	constexpr int fixed = 40;
	//ちぎりが発生した時のフレーム硬直
	constexpr std::array<int, 12> thigiri = {
		0,19,24,28,31,34,37,40,42,44,46,48
	};
	//連鎖で落ちる段差によってフレーム硬直
	constexpr std::array<int, 13> rensa = {
		55,80,82,84,86,88,90,92,94,96,98,100,102
	};
	//おじゃまぷよが落ちる場所によるフレーム硬直
	constexpr std::array<int, 78> ojamaPosition = {
		28,27,29,26,28,26,
		32,31,34,30,33,30,
		36,35,38,33,37,34,
		39,38,41,36,40,37,
		42,41,44,39,43,40,
		45,44,47,42,46,42,
		47,46,50,44,49,45,
		50,48,53,46,51,47,
		52,51,55,48,53,49,
		54,53,57,50,56,52,
		56,55,60,52,58,54,
		58,57,62,54,60,56,
		60,59,64,56,62,58
	};
	//おじゃまの量によるフレーム硬直
	//条件はwebサイトから
	int ojamaCnt(int ojama);
	//時間をフレームに変換する、余剰分の時間は残して引き算する
	int timeToFrames(double& time);
	//時間をフレームに変換する
	int toFrames(const double time);
}
