#include "Stage.hpp"
#include "core.hpp"

namespace Stage {

	int Y_o() {
		return 60;
	}
	int X_o(bool isTwoPlayer) {
		int oneP = Scene::Center().x - Stage::lengthW * 2;
		int twoP = Scene::Center().x + Stage::lengthW;
		return (isTwoPlayer ? twoP : oneP);
	}

	void draw(bool isTwoPlayer) {
		const int siz = blockSize;
		const int offset = siz + blockOffset;
		//外部の壁の描画
		Rect{ X_o(isTwoPlayer) - 10,Y_o() - 10,lengthW + 20,lengthH + 20 }.draw(wallColor);


		constexpr ColorF backColor = ColorF{ 0.8 };
		//ブロックの描画(14段分)
		for (auto y : step(height)) {
			for (auto x : step(width)) {
				Rect{ x * offset + X_o(isTwoPlayer) ,y * offset + Y_o(),siz }
				.draw(backColor);
			}
		}
		//予告スペースの描画
		//TODO:描画を分かりやすくする

		//ブロック２マス分の背景描画
		Rect{ X_o(isTwoPlayer),Y_o(), offset * width, offset * 2 }
		.draw(wallColor);
		//左上からブロック一個分のオフセットでおじゃま予告を描画
		Rect{ X_o(isTwoPlayer), Y_o() + offset, yokokuLengthW, yokokuLengthH }
		.draw(ColorF{ 0.8 });

		//×印の描画

	}

}
