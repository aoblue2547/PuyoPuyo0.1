#include <Siv3D.hpp> // OpenSiv3D v0.6.5
#include "core.hpp"
#include "GameManager.hpp"
#include "AIlist.hpp"
#include "Pulldown.hpp"

/*
TODO:降ることが確定していても相殺で消せるようにする
TODO:たまに操作ぷよが画面外に出る不具合を直す
	(回転してからすぐに左右移動をするとぷよが画面外に出る)
TODO:GameStateを作る
TODO:フレームをeSport仕様にする
*/




void Main() {

	/*-----------------環境設定-------------------*/

	Window::Resize(1000, 600);
	Scene::SetBackground(BackgroundColor);

	TextureAsset::Register(U"puyo", U"puyo_sozai.png");
	TextureAsset::Register(U"ojama", U"yokokuOjamaPuyo.png");
	FontAsset::Register(U"Rensa", 20);
	FontAsset::Register(U"17", 17);

	/*-----------------変数宣言-------------------*/

	GameManager game(playeris::keyboard, playeris::keyboard);

	String haipuyoNumber = ToString(game.tumoNumber());
	const Rect inputArea{ Arg::center(Scene::Center().x,450),100,30 };

	const Array<String> list = { U"Keyboard",U"ProController",U"AI" };
	//TODO:多分左右対称にパラメータ直打ちでなくできるはず
	GUI::Pulldown p1list{ list,FontAsset(U"17"),Point{Scene::Center().x - 160,500} };
	GUI::Pulldown p2list{ list,FontAsset(U"17"),Point(Scene::Center().x + 20,500) };

	AIlist AIlist;

	GUI::Pulldown p1AIPulldown(AIlist.AInameArray(), FontAsset(U"17"), Point{ Scene::Center().x - 160,350 });
	GUI::Pulldown p2AIPulldown(AIlist.AInameArray(), FontAsset(U"17"), Point{ Scene::Center().x + 80,350 });

	

	/*-----------------Main loop-----------------*/

	while (System::Update()) {


		/*--------------------AIの登録-----------------*/
		if (SimpleGUI::ButtonAt(U"Register", Vec2{ Scene::Center().x,250 }, unspecified, !game.nowPlaying())) {
			Optional<FilePath> path = Dialog::OpenFile({ FileFilter::AllFiles() });
			if (path) {
				const int row = AIlist.size();
				String name = path.value().reversed();
				name = strings::parse(name, U'/').first.reversed();
				name = strings::parse(name, U'.').first;

				AIlist.addAI(name, path.value());

				p1AIPulldown.pushItem(name);
				p2AIPulldown.pushItem(name);
				
			}
		}

		/*-------------------GameManager関連-----------------*/

		if (SimpleGUI::ButtonAt(U"Start", Vec2{Scene::Center().x,300},unspecified,!game.nowPlaying())) {
			game.resume();
		}
		if (SimpleGUI::ButtonAt(U"Stop", Vec2{ Scene::Center().x,350 },unspecified,game.nowPlaying())) {
			game.stop();
		}
		if (SimpleGUI::ButtonAt(U"Reset", Vec2{ Scene::Center().x,400})) {
			game.reset();
		}

		/*--------------------配ぷよ関連----------------*/

		TextInput::UpdateText(haipuyoNumber);
		//TODO:数字しか入力できないようにする(したい)
		if (haipuyoNumber.size() != 0 and (*haipuyoNumber.rbegin() - U'0' < 0 or *haipuyoNumber.rbegin() - U'0' >= 10)) {
			haipuyoNumber.pop_back();
		}
		const String editingText = TextInput::GetEditingText();
		inputArea.draw(ColorF{ 0.3 });
		FontAsset(U"Rensa")(haipuyoNumber + U'|' + editingText).draw(inputArea.stretched(0));
		if (KeyEnter.down() and !game.nowPlaying()) {
			game.useTumo(Parse<int>(haipuyoNumber));
		}
		FontAsset(U"Rensa")(U"ツモ番号 = {}"_fmt(game.tumoNumber())).draw(20, 10, Palette::Black);

		/*----------------player関連-------------------*/

		p1list.update();
		p1list.draw();
		p2list.update();
		p2list.draw();
		game.setPlayer(0, (playeris)p1list.getIndex());
		game.setPlayer(1, (playeris)p2list.getIndex());

		p1AIPulldown.update();
		p1AIPulldown.draw();
		p2AIPulldown.update();
		p2AIPulldown.draw();


		game.runLoop();

	}



}

