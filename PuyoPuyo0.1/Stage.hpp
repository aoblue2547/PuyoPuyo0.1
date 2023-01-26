#pragma once
//ステージのマスの数、高さ、描画するときの左上の座標など

namespace Stage {
	//1P,2P共通の設定
	inline int width = 6;
	inline int height = 14;
	//描画上の設定(1P,2P共通)
	inline const int blockSize = 32;
	inline const int blockOffset = 0;
	inline int lengthW = width * (blockSize + blockOffset);
	inline int lengthH = height * (blockSize + blockOffset);
	inline int yokokuLengthW = lengthW;
	inline int yokokuLengthH = blockSize + blockOffset;//後で変えるかもしれない
	inline ColorF wallColor = ColorF{ 0.7,0.8,1.0 };
	//描画上の設定(1P,2P別々)
	int Y_o();
	int X_o(bool isTwoPlayer);

	//実際にステージを描画する関数
	void draw(bool isTwoPlayer);
}
