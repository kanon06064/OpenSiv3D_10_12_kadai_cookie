# include <Siv3D.hpp>

/// @brief アイテムのボタン
/// @param rect ボタンの領域
/// @param texture ボタンの絵文字
/// @param font 文字描画に使うフォント
/// @param name アイテムの名前
/// @param desc アイテムの説明
/// @param count アイテムの所持数
/// @param enabled ボタンを押せるか
/// @return ボタンが押された場合 true, それ以外の場合は false
bool Button(const Rect& rect, const Texture& texture, const Font& font, const String& name, const String& desc, int32 count, bool enabled)
{
	if (enabled)
	{
		rect.draw(ColorF{ 0.3, 0.5, 0.9, 0.8 });

		rect.drawFrame(2, 2, ColorF{ 0.5, 0.7, 1.0 });

		if (rect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}
	else
	{
		rect.draw(ColorF{ 0.0, 0.4 });

		rect.drawFrame(2, 2, ColorF{ 0.5 });
	}

	texture.scaled(0.5).drawAt(rect.x + 50, rect.y + 50);

	font(name).draw(30, rect.x + 100, rect.y + 15, Palette::White);

	font(desc).draw(18, rect.x + 102, rect.y + 60, Palette::White);

	font(count).draw(50, Arg::rightCenter((rect.x + rect.w - 20), (rect.y + 50)), Palette::White);

	return (enabled && rect.leftClicked());
}

void Main()
{
	// クッキーの絵文字
	const Texture texture{ U"🍪"_emoji };

	// 農場の絵文字
	const Texture farmEmoji{ U"🌾"_emoji };

	// 工場の絵文字
	const Texture factoryEmoji{ U"🏭"_emoji };



	const Texture textureMonster{ U"example/bosses_transparent.png" };
	const Texture textureMonster2{ U"example/monster_transparent.png" };
	const Texture textureMonster3{ U"example/sura2_transparent.png" };
	const Texture textureMonster4{ U"example/ganpega.png" };
	const Texture textureMonster5{ U"example/monster5_2.png" };


	// フォント
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	// クッキーのクリック円
	const Circle monsterCircle{ 170, 300, 100 };

	// クッキーの表示サイズ（倍率）
	double monsterScale = 1.5;

	// クッキーの個数
	double monster = 0;

	// 農場の所有数
	int32 farmCount = 0;

	// 工場の所有数
	int32 factoryCount = 0;

	//牧場の所有数
	int32 bokujouCount = 0;

	//魔門の所有数
	int32 mamonCount = 0;

	// 農場の価格
	int32 farmCost = 10;

	// 工場の価格
	int32 factoryCost = 50;

	//牧場の価格
	int32 bokujouCost = 200;

	//魔門の価格
	int32 mamonCost = 1000;

	// ゲームの経過時間の蓄積
	double accumulatedTime = 0.0;

	while (System::Update())
	{
		// クッキーの毎秒の生産量 (cookies per second) を計算する
		const int64 cps = (farmCount + factoryCount  * 10  + bokujouCount * 30 + mamonCount * 100) ;
		

		// ゲームの経過時間を加算する
		accumulatedTime += Scene::DeltaTime();


		// 0.1 秒以上蓄積していたら
		if (0.1 <= accumulatedTime)
		{
			accumulatedTime -= 0.1;

			// 0.1 秒分のクッキー生産を加算する
			monster += (cps * 0.1);
			
		}

		// 農場の価格を計算する
		farmCost = 10 + (farmCount * 10);

		// 工場の価格を計算する
		factoryCost = 50 + (factoryCount * 50);

		//牧場の価格を計算する
		bokujouCost = 200 + (bokujouCount * 200);

		//魔門の価格を計算する
		mamonCost = 1000 + (mamonCount * 1000);

		// クッキー円上にマウスカーソルがあれば
		if (monsterCircle.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		// クッキー円が左クリックされたら
		if (monsterCircle.leftClicked())
		{
			monsterScale = 1.3;
			++monster;
		}

		// クッキーの表示サイズを回復する
		monsterScale += Scene::DeltaTime();

		if (1.5 < monsterScale)
		{
			monsterScale = 1.5;
		}

		// 背景を描く
		Rect{ 0, 0, 800, 600 }.draw(Arg::top = ColorF{ 0, 0, 0 }, Arg::bottom = ColorF{ 1, 1, 1 });

		// クッキーの数を整数で表示する
		font(U"{:.0f}"_fmt(monster)).drawAt(60, 170, 100);

		// クッキーの生産量を表示する
		font(U"毎秒: {}"_fmt(cps)).drawAt(24, 170, 160);

		// クッキーを描画する
		textureMonster.scaled(monsterScale).drawAt(monsterCircle.center);

		// 農場ボタン
		if (Button(Rect{ 340, 40, 420, 100 }, textureMonster3, font, U"スライム農場", U"C{} / 1 CPS"_fmt(farmCost), farmCount, (farmCost <= monster)))
		{
			monster -= farmCost;
			++farmCount;
		}

		// 工場ボタン
		if (Button(Rect{ 340, 160, 420, 100 }, textureMonster2, font, U"レイス工場", U"C{} / 10 CPS"_fmt(factoryCost), factoryCount, (factoryCost <= monster)))
		{
			monster -= factoryCost;
			++factoryCount;
		}

		if (Button(Rect{ 340, 280, 420, 100 }, textureMonster4, font, U"ペガサス牧場", U"C{} / 30 CPS"_fmt(bokujouCost), bokujouCount, (bokujouCost <= monster)))
		{
			monster -= bokujouCost;
			++bokujouCount;
		}

		if (Button(Rect{ 340, 400, 420, 100 }, textureMonster5, font, U"魔門の生成", U"C{} / 100 CPS"_fmt(mamonCost), mamonCount, (mamonCost <= monster)))
		{
			monster -= mamonCost;
			++mamonCount;
		}
	}
}
