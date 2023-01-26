#include "core.hpp"


TextureRegion PuyoChip::get(int index) const {
	const int x = (index % 6) * chipsize;
	const int y = (index / 6) * chipsize;
	return TextureAsset(U"puyo")(x, y, chipsize);
}

//テキストファイルのツモをPuyo型に変換する
Puyo txtToPuyo(char c) {
	Puyo p;
	switch (c) {
	case 'r':
		p = Puyo::Red;
		break;
	case 'g':
		p = Puyo::Green;
		break;
	case 'b':
		p = Puyo::Blue;
		break;
	case 'y':
		p = Puyo::Yellow;
		break;
	case 'p':
		p = Puyo::Purple;
		break;
	default:
		p = Puyo::Red;
		break;
	}
	return p;
}

void boolKey::update(bool b) {
	if (keyup) {
		duration = 0;
	}
	keyup = false;
	keydown = false;
	if (b) {
		if (not keypressed) {
			keydown = true;
		}
		duration += Scene::DeltaTime();
	}
	else {
		if (keypressed) {
			keyup = true;
		}
	}
	keypressed = b;
}
