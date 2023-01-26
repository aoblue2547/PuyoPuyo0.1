#pragma once
//いろいろなソースで使うものの集まり

inline ColorF BackgroundColor = ColorF{ 1.0 };
//方向を全探索するやつ、puyoField上では上、右、下、左
inline int dirc[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };

//ぷよの定義
enum Puyo {
	Red, Green, Blue, Yellow, Purple, Ojama, Wall, Back
};

//ぷよを描画するためのキャラチップ
class PuyoChip {
	static constexpr int chipsize = 32;
public:
	TextureRegion get(int index) const;
};

//テキストファイルのツモをPuyo型に変換する
Puyo txtToPuyo(char c);

//操作するのが何であるかの定義
enum playeris {
	keyboard,procon,AI
};

//得点計算のときに使うやつ
namespace Score {
	//全消しボーナス
	inline const int zenkeshi = 2100;
	//何連鎖目かでのボーナス
	inline const int rensaBonus[19] = { 0,8,16,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512 };
	//連結数4から11以上まで
	inline const int renketuBonus[] = { 0,2,3,4,5,6,7,10 };
	//多色ボーナス
	inline const int colorBonus[] = { 0,3,6,12,24 };
	//得点計算(ぷよの数*10(連鎖ボーナス+連結ボーナス+多色ボーナス))
	inline int rensaScore(int rensa,int puyocnt,Array<int> renketu,Array<bool> eraseColor) {
		int a = rensaBonus[rensa];
		int b = 0;
		for (int i = 0; i < 8; ++i) {
			b += renketuBonus[i] * renketu[i];
		}
		int c = colorBonus[eraseColor.count(true) - 1];
		return puyocnt * 10 * Max(a + b + c, 1);
	}
}

//bool型をOpenSiv3DのkeyInput型のように使用できるクラス
class boolKey {
private:
	bool keydown, keypressed, keyup;
	double duration;
public:
	boolKey() = default;
	void update(bool);
	bool down() const { return keydown; };
	bool up()const { return keyup; };
	bool pressed()const { return keypressed; };
	double pressedDuration()const { return duration; };
};

namespace strings {
	inline std::pair<String, String> parse(String s, char div) {
		const int divider = (int)s.indexOf(div, 0);
		String first = s.substr(0, divider);
		String second = s.substr(divider + 1);
		return { first,second };
	}
}
