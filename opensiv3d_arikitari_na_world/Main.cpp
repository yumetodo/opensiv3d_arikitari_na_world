
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
using App = SceneManager<String>;
class Title : public App::Scene
{
private:
	FontAsset font{ U"TitleFont" };
	Audio bgm = AudioAsset(U"title");
public:
	Title(const InitData& init) : IScene(init)
	{
		this->bgm.setLoop(true);
		this->bgm.play();
	}
	~Title() noexcept
	{
		this->bgm.stop();
	}

	// 更新関数
	void update() override
	{
		// 左クリックで
		if (MouseL.down())
		{
			// ゲームシーンに遷移
			changeScene(U"Game");
		}
	}

	// 描画関数 (const 修飾)
	void draw() const override
	{
		Scene::SetBackground(ColorF(0.3, 0.4, 0.5));

		font(U"My Game").drawAt(400, 100);

		Circle(Cursor::Pos(), 50).draw(Palette::Orange);
	}
};
class Game : public App::Scene {
private:
	Font font{ 60 };
	Texture cat{ Emoji(U"🐈") };
	Vec2 catPos{ 640, 450 };
	Audio bgm = AudioAsset(U"flower garden");
	inline static constexpr Vec2 button_pos{ 600, 20 };
public:
	Game(const InitData& init) : IScene(init)
	{
		this->bgm.setLoop(true);
		this->bgm.play();
	}
	~Game() noexcept
	{
		this->bgm.stop();
	}
	virtual void update() override
	{
		// ボタンが押されたら
		if (SimpleGUI::Button(U"Move the cat", button_pos))
		{
			// 猫の座標を画面内のランダムな位置に移動する
			catPos = RandomVec2(Scene::Rect());
		}
		else if (MouseL.down())
		{
			// タイトルシーンに遷移
			changeScene(U"Title");
		}
		// [A] キーが押されたら
		if (KeyA.down())
		{
			// Hello とデバッグ表示する
			Print << U"Hello!";
		}
	}
	virtual void draw() const override
	{
		Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
		// テキストを画面の中心に描く
		font(U"ありきたりな世界, Siv3D!🐣").drawAt(Scene::Center(), Palette::Black);
		// 大きさをアニメーションさせて猫を表示する
		cat.resized(100 + Periodic::Sine0_1(1s) * 20).drawAt(catPos);
		// マウスカーソルに追従する半透明の赤い円を描く
		Circle(Cursor::Pos(), 40).draw(ColorF(1, 0, 0, 0.5));
	}
};
void Main()
{
	AudioAsset::Register(U"title", U"title.wav");
	AudioAsset::Register(U"flower garden", U"flower garden.wav");
	for (auto&& name : { U"title", U"flower garden" }) AudioAsset::Preload(name);
	App manager;
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");
	while (System::Update())
	{
		if (!manager.update()) break;
	}
}

