# include <Siv3D.hpp>
# include "BunchoUI.hpp"

using namespace BunchoUI;

struct ItemData
{
	String name;
	int32 amount;
	Texture texture;
	int32 count = 0;
};

auto AmountDisplay(int32 amount, double size, const ColorF& textColor = Palette::Black)
{
	return Row::Create
	({
		.relative = none,
		.children
		{
			TextureUI::Create({.texture = TextureAsset{U"Coin"}.resized(size * 1.2)}),
			TextUI::Create({.text = U"×{}"_fmt(amount),.fontSize = size,.color = textColor})
		}
	});
}

auto ItemCountDisplay(int32 count,double size)
{
	return TextUI::Create({ .text = U"所持：{}"_fmt(count),.fontSize = size });
}

auto ItemButton(const ItemData& item, bool canBuy)
{
	return SimpleButton::Create
	({
		.color = canBuy ? ColorF{1} : ColorF{0.8},
		.padding = 5,
		.margine = 5,
		.width = 250,
		.child = Row::Create
		({
			.children
			{
				TextureUI::Create({.texture = item.texture.resized(70),}),
				Column::Create
				({
					.margine = 10,
					.flex = 1.0,
					.children
					{
						TextUI::Create({.text = item.name}),
						Row::Create
						({
							.mainAxis = MainAxis::spaceBetween,
							.children
							{
								AmountDisplay(item.amount,18,canBuy ? Palette::Black : Palette::Red),
								ItemCountDisplay(item.count,16)
							}
						})
					}
				}),
			}
		})
	});
}

auto PlayerCoinsDisplay(int32 coins)
{
	return Row::Create
	({
		.children
		{
			TextUI::Create({.text = U"所持金 ",.fontSize = 25}),
			AmountDisplay(coins,40)
		}
	});
}

auto ItemDetailsDisplay(const ItemData& item, const std::shared_ptr<UIElement>& buyButton, bool canBuy)
{
	return Column::Create
	({
		.children
		{
			TextureUI::Create({.texture = item.texture,.flex = 1.0,.relative = Relative::Stretch()}),
			TextUI::Create({.text = item.name,.fontSize = 40}),
			TextUI::Create({U"とってもおいしいくだもの"}),
			Row::Create
			({
				.mainAxis = MainAxis::spaceEvenly,
				.children
				{
					AmountDisplay(item.amount,30,canBuy ? Palette::Black : Palette::Red),
					ItemCountDisplay(item.count,30)
				}
			}),
			buyButton
		}
	});
}

auto PurchaseDialog(const ItemData& item, const std::function<void()>& onClicked)
{
	auto okButton = SimpleButton::Create
	({
		.color = Palette::Orange,
		.mouseOverColor = Palette::Orangered,
		.margine = 10,
		.width = 120,
		.child = TextUI::Create({.text = U"購入",.color = Palette::White})
	});

	auto cancelButton = SimpleButton::Create
	({
		.color = Palette::Hotpink,
		.mouseOverColor = Palette::Deeppink,
		.margine = 10,
		.width = 120,
		.child = TextUI::Create({.text = U"キャンセル",.color = Palette::White})
	});

	return SimpleDialog::Create({
		.child = RectPanel::Create
		({
			.color = Palette::White,
			.child = Column::Create
			({
				.children
				{
					Column::Create
					({
						.margine = 20,
						.children
						{
							TextUI::Create({.text = U"{}を購入しますか？"_fmt(item.name)}),
							AmountDisplay(item.amount,20),
						}
					}),
					Row::Create
					({
						.children
						{
							okButton,cancelButton
						}
					})
				}
			})
		}),
		.updateFunc = [=](SimpleDialog* ptr)
		{
			if (okButton->clicked())
			{
				ptr->close();
				onClicked();
			}
			if (cancelButton->clicked())
			{
				ptr->close();
			}
		}
	});
}

void Main()
{
	Window::SetStyle(WindowStyle::Sizable);

	Scene::SetBackground(Palette::Lightpink);

	FontAsset::Register(U"Font", FontMethod::MSDF, 20);
	TextUI::DefaultFontName = U"Font";

	TextureAsset::Register(U"Coin", U"💎"_emoji);

	int32 playerCoins = 100;

	size_t selectIndex = 0;

	Array<ItemData>items
	{
		{U"なし",20,Texture{U"🍐"_emoji}},
		{U"レモン",20,Texture{U"🍋"_emoji}},
		{U"バナナ",20,Texture{U"🍌"_emoji}},
		{U"キウイフルーツ",20,Texture{U"🥝"_emoji}},
		{U"パイナップル",30,Texture{U"🍍"_emoji}},
		{U"ココナッツ",30,Texture{U"🥥"_emoji}},
		{U"もも",40,Texture{U"🍑"_emoji}},
		{U"マンゴー",50,Texture{U"🥭"_emoji}},
		{U"いちご",60,Texture{U"🍓"_emoji}},
		{U"さくらんぼ",60,Texture{U"🍒"_emoji}},
		{U"りんご",80,Texture{U"🍎"_emoji}},
		{U"青りんご",80,Texture{U"🍏"_emoji}},
		{U"みかん",80,Texture{U"🍊"_emoji}},
		{U"ぶどう",100,Texture{U"🍇"_emoji}},
		{U"スイカ",130,Texture{U"🍉"_emoji}},
		{U"メロン",200,Texture{U"🍈"_emoji}}
	};

	UIManager manager;

	auto itemColumn = Column::Create({});

	for (const auto& item : items)
	{
		itemColumn->addChild(ItemButton(item, item.amount <= playerCoins));
	}

	auto buyButton = SimpleButton::Create
	({
		.color = Palette::Orange,
		.mouseOverColor = Palette::Orangered,
		.margine = 20,
		.child = Row::Create
		({
			.margine = Margin{0,20},
			.children
			{
				TextureUI::Create({.texture = Texture{0xf0110_icon,30}}),
				TextUI::Create({.text = U"買う",.fontSize = 30,.color = Palette::White,.margine = 5})
			}
		})
	});

	auto playerCoinsPanel = RectPanel::Create
	({
		.r = 20,
		.margine = 10,
		.relative = Relative::Stretch(),
		.child = PlayerCoinsDisplay(playerCoins)
	});

	auto itemDetailsPanel = RectPanel::Create
	({
		.r = 20,
		.margine = 10,
		.flex = 1.0,
		.relative = Relative::Stretch(),
		.child = ItemDetailsDisplay(items[selectIndex],buyButton,items[selectIndex].amount <= playerCoins),
	});

	manager.setChildren
	({
		Row::Create
		({
			.margine = 10,
			.children
			{
				SimpleScrollbar::Create
				({
					.margine = 10,
					.child = itemColumn,
				}),
				Column::Create
				({
					.flex = 1.0,
					.children
					{
						playerCoinsPanel,
						itemDetailsPanel,
					}
				}),
			}
		})
	});

	while (System::Update())
	{
		manager.update();

		for (size_t i = 0; i < items.size(); ++i)
		{
			if (itemColumn->getChildren()[i]->clicked())
			{
				selectIndex = i;
				itemDetailsPanel->setChild(ItemDetailsDisplay(items[i], buyButton, items[i].amount <= playerCoins));
				buyButton->clickable = items[i].amount <= playerCoins;
			}
		}

		if (buyButton->clicked())
		{
			manager.addChild(PurchaseDialog(
				items[selectIndex],
				[&]
				{
					playerCoins -= items[selectIndex].amount;
					++items[selectIndex].count;

					playerCoinsPanel->setChild(PlayerCoinsDisplay(playerCoins));
					itemDetailsPanel->setChild(ItemDetailsDisplay(items[selectIndex], buyButton, items[selectIndex].amount <= playerCoins));

					buyButton->clickable = items[selectIndex].amount <= playerCoins;

					for (size_t i = 0; i < items.size(); ++i)
					{
						const bool canBuy = items[i].amount <= playerCoins;
						if (not canBuy || i == selectIndex)
						{
							itemColumn->replaceChild(i, ItemButton(items[i], canBuy));
						}
					}
				}
			));
		}

		manager.draw();
	}
}
