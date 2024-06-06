# include <Siv3D.hpp>
# include "BunchoUI.hpp"

using namespace BunchoUI;

auto TextureButton(const Texture& texture, StringView name) {
	return SimpleButton::Create({
		.child = Column::Create({
			.crossAxis=CrossAxis::center,
			.children{
				TextureUI::Create({.texture = texture.resized(100)}),
				TextUI::Create({.text=name})
			}
		})
	});
}

void Main()
{
	Window::SetStyle(WindowStyle::Sizable);

	Scene::SetBackground(Palette::White);

	FontAsset::Register(U"Font", FontMethod::MSDF, 20);

	TextUI::DefaultFontName = U"Font";

	Texture apple{ U"🍎"_emoji };
	Texture orange{ U"🍊"_emoji };
	Texture banana{ U"🍌"_emoji };

	UIManager manager;

	auto appleButton = TextureButton(apple, U"りんご");
	auto orangeButton = TextureButton(orange, U"みかん");
	auto bananaButton = TextureButton(banana, U"ばなな");
	auto fruitRow = Row::Create({});
	auto slider = SimpleSlider::Create({});

	auto menuPanel = RectPanel::Create({
		.padding = 30,
		.relative = Relative::Stretch(),
		.child = Column::Create({
			.mainAxis = MainAxis::spaceAround,
			.crossAxis = CrossAxis::center,
			.children{
				appleButton,
				orangeButton,
				bananaButton,
				fruitRow,
				slider
			}
		}),
	});

	auto homeButton = SimpleButton::Create({
		.color = Palette::Orange,
		.mouseOverColor = Palette::Orangered,
		.margine = 20,
		.relative = Relative::BottomRight(),
		.child = TextureUI::Create({.texture = Texture{0xF015_icon,50}})
	});

	manager.setChildren({
		Row::Create({
			.children{
				menuPanel,
				SimpleScrollbar::Create({
					.flex=1.0,
					.child = Column::Create({
						.margine = 30,
						.children{
							TextUI::Create({.text = U"BunchoUI",.fontSize=50,.relative=Relative::Center()}),
							TextUI::Create({.text = U"BunchoUIはソースコード上で簡単にレイアウト定義を行えるUIシステムです。\nUIの自作も簡単にできます。"}),
							TextUI::Create({.text = U"なぜこの名前にしたかと言うと、検索時に被らないユニークな名前にしたかったからです。"}),
							TextUI::Create({.text = U"噓です。文鳥がかわいいからです。",.fontSize=30,.color = Palette::Red,.relative = Relative::Center()}),
							TextUI::Create({.text = U"このシステムには全く文鳥は関係ないですが、文鳥がかわいいので仕方がないです。"})
						}
					})
				})
			},
		}),
		homeButton
	});

	while (System::Update())
	{
		manager.update();

		if (appleButton->clicked())
		{
			fruitRow->addChild(TextureUI::Create({ .texture = apple.resized(50) }));
		}

		if (orangeButton->clicked())
		{
			fruitRow->addChild(TextureUI::Create({ .texture = orange.resized(50) }));
		}

		if (bananaButton->clicked())
		{
			fruitRow->addChild(TextureUI::Create({ .texture = banana.resized(50)}));
		}

		menuPanel->color = HSV{ slider->value * 360,0.3};

		if (homeButton->clicked())
		{
			Window::Resize(800, 600);
		}

		manager.draw();
	}
}
