#include"BunchoUI.hpp"

using namespace BunchoUI;

//-----------------------------------------------
//  CursorSystem
//-----------------------------------------------

namespace
{
	bool g_mouseCaptured = false;

	bool g_mouseCapturedOneFrame = false;

	bool g_wheelCaptured = false;

	bool g_wheelHCaptured = false;

	String g_handStyleName;
}

void CursorSystem::SetCapture(const bool capture)
{
	g_mouseCaptured = capture;
}

void CursorSystem::SetCaptureOneFrame(const bool capture)
{
	g_mouseCapturedOneFrame = capture;
}

bool CursorSystem::IsCaptured()
{
	return g_mouseCaptured or g_mouseCapturedOneFrame;
}

void CursorSystem::setWheelCapture(const bool capture)
{
	g_wheelCaptured = capture;
}

void CursorSystem::setWheelHCapture(const bool capture)
{
	g_wheelHCaptured = capture;
}

bool CursorSystem::IsWheelCaptured()
{
	return g_wheelCaptured;
}

bool CursorSystem::IsWheelHCaptured()
{
	return g_wheelHCaptured;
}

void CursorSystem::SetHandStyle(StringView name)
{
	g_handStyleName = name;
}

void CursorSystem::RequestHandStyle()
{
	if (g_handStyleName)
	{
		Cursor::RequestStyle(g_handStyleName);
	}
	else
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}
}

//-----------------------------------------------
//  UIElement
//-----------------------------------------------

UIElement::~UIElement()
{
	if (m_hasMouseCapture)
	{
		CursorSystem::SetCapture(false);
	}
}

bool UIElement::update()
{
	m_isAvailableCursor = (not CursorSystem::IsCaptured()) or hasMouseCapture();

	m_mouseOvered = m_isAvailableCursor and onMouseOver();

	if (m_mouseOvered and clickable)
	{
		CursorSystem::RequestHandStyle();
	}

	onUpdate();

	if (m_mouseOvered)
	{
		//このフレームはキャプチャされた状態にする
		CursorSystem::SetCaptureOneFrame(true);
	}

	if (m_changeSizeFlg)
	{
		m_changeSizeFlg = false;
		return true;
	}
	else
	{
		return false;
	}
};

void UIElement::build(const RectF& rect, const Relative& parentRelative) {

	Relative relative = parentRelative;

	if (m_relative)
	{
		relative = m_relative.value();
	}

	RectF area = rect;
	area.x += m_margine.left;
	area.y += m_margine.top;
	area.w -= m_margine.getHorizontal();
	area.h -= m_margine.getVertical();

	SizeF size = getSizeWithoutMargine();

	if (area.w < size.x)
	{
		size.x = area.w;
		size.y = getYWithoutMargine(area.w);
	}
	else if (area.h < size.y)
	{
		size.x = getXWithoutMargine(area.h);
		size.y = area.h;
	}

	if (relative.x.stretch)
	{
		size.x = area.w;
	}
	if (relative.y.stretch)
	{
		size.y = area.h;
	}

	RectF result{};
	result.x = area.x + (area.w - size.x) * relative.x.relative;
	result.y = area.y + (area.h - size.y) * relative.y.relative;
	result.size = size;

	m_rect = result;
	onBuild();
}

void UIElement::onMoveBy([[maybe_unused]] const Vec2& pos)
{
	onBuild();
}

void UIElement::onUpdate()
{

}

void UIElement::onDraw([[maybe_unused]] const RectF& rect)const
{

}

void UIElement::onBuild()
{

};

double UIElement::onGetX([[maybe_unused]] double y)const
{
	return onGetSize().x;
}

double UIElement::onGetY([[maybe_unused]] double x)const
{
	return onGetSize().y;
}

bool UIElement::onMouseOver()const
{
	return getRect().mouseOver();
}

//-----------------------------------------------
//  NoneUI
//-----------------------------------------------

void NoneUI::onDraw(const RectF&)const
{
	RectF rect = getRect();
	rect.drawFrame(2, Palette::Red);
	Line{ rect.tl(),rect.br() }.draw(2, Palette::Red);
	Line{ rect.tr(),rect.bl() }.draw(2, Palette::Red);
}

SizeF NoneUI::onGetSize()const
{
	return SizeF{ 20,20 };
}

//-----------------------------------------------
//  Row
//-----------------------------------------------
double Row::onGetX(double y)const
{
	return onGetMain(y);
}

double Row::onGetY(double x)const
{
	return onGetCross(x);
}

//-----------------------------------------------
//  Column
//-----------------------------------------------
double Column::onGetX(double y)const
{
	return onGetCross(y);
}

double Column::onGetY(double x)const
{
	return onGetMain(x);
}

//-----------------------------------------------
//  PanelBase
//-----------------------------------------------

void PanelBase::onBuild()
{
	RectF area = getRect();
	area.x += m_padding.left;
	area.y += m_padding.top;
	area.w -= m_padding.getHorizontal();
	area.h -= m_padding.getVertical();
	child->build(area);
}

void PanelBase::onMoveBy(const Vec2& pos) {
	child->moveBy(pos);
}

SizeF PanelBase::onGetSize()const
{
	return child->getSize() + m_padding.getSize();
}

double PanelBase::onGetX(double y)const
{
	return child->getX(y - m_padding.getVertical()) + m_padding.getHorizontal();
}

double PanelBase::onGetY(double x)const
{
	return child->getY(x - m_padding.getHorizontal()) + m_padding.getVertical();
}

//-----------------------------------------------
//  RectPanel
//-----------------------------------------------

void RectPanel::onUpdate()
{
	childUpdate();
}

void RectPanel::onDraw(const RectF& drawingArea)const
{
	getRect().draw(color);
	childDraw(drawingArea);
}

//-----------------------------------------------
//  SimpleButton
//-----------------------------------------------

void SimpleButton::onUpdate()
{
	childUpdate();
}

void SimpleButton::onDraw(const RectF& drawingArea)const
{
	const Transformer2D transform{ pressed()?Mat3x2::Scale(0.95,getRect().center()):Mat3x2::Identity()};
	getRoundRect().draw((mouseOver() and clickable) ? mouseOverColor : color);
	childDraw(drawingArea);
	if (not clickable)
	{
		getRoundRect().draw(ColorF{ 0,0.5 });
	}
}

bool SimpleButton::onMouseOver()const
{
	return getRoundRect().mouseOver();
}

//-----------------------------------------------
//  SimpleSlider
//-----------------------------------------------

bool SimpleSlider::onMouseOver()const {
	const RectF rect = getRect();
	return RectF{ Arg::center = rect.center(),rect.w, KnobR * 2 }.mouseOver();
}

void SimpleSlider::onUpdate()
{

	if (enabled and isAvailableCursor())
	{
		const RectF rect = getRect();
		const RectF hitBox{ Arg::center = rect.center(),rect.w, KnobR * 2 };

		if (hasMouseCapture())
		{
			CursorSystem::RequestHandStyle();
			const double w = rect.w - KnobR * 2;
			value = Clamp(Cursor::PosF().x - (rect.x+ KnobR), 0.0, w) / w;
			if (MouseL.up())
			{
				setMouseCapture(false);
			}
		}
		else {
			if (hitBox.mouseOver())
			{
				CursorSystem::RequestHandStyle();
			}
			if (hitBox.leftClicked())
			{
				setMouseCapture(true);
			}
		}
	}
}

void SimpleSlider::onDraw(const RectF&)const
{
	const RectF rect = getRect();
	const double w = rect.w - KnobR * 2;
	RectF{ Arg::center = rect.center(),w,BarHeight }.draw(Palette::Gray);
	const double length = value * w;
	RectF{ Arg::center = rect.leftCenter() + Vec2{length / 2+ KnobR,0},length,BarHeight }.draw(color);
	if (not enabled) {
		RectF{ Arg::center = rect.center(),w,BarHeight }.draw(ColorF{ 0,0.4 });
	}
	Circle{ length + rect.x+ KnobR, rect.center().y ,KnobR }.draw(enabled ? Palette::White : Palette::Lightgray);
	if (not enabled) {
		Circle{ length + rect.x+ KnobR, rect.center().y ,KnobR }.draw(ColorF{ 0,0.4 });
	}
}

SizeF SimpleSlider::onGetSize()const
{
	return { 150+ KnobR * 2,KnobR * 2 };
}

//-----------------------------------------------
//  SimpleScrollbar
//-----------------------------------------------

void SimpleScrollbar::onUpdate()
{

	RectF view = getRect();

	bool changeValueFlg = false;

	if (isScroll())
	{
		view.w -= ScrollbarWidth;

		if (isAvailableCursor())
		{

			if (hasMouseCapture())
			{
				if (MouseL.up())
				{
					setMouseCapture(false);
				}

				const double space = view.h * (1 - getRate());
				double oldValue = m_value;
				m_value = Clamp(m_value + Cursor::DeltaF().y / space, 0.0, 1.0);

				if (oldValue != m_value)
				{
					changeValueFlg = true;
				}
			}
			else {

				if (getBarRoundRect().mouseOver())
				{
					CursorSystem::RequestHandStyle();
				}

				if (getBarRoundRect().leftClicked())
				{
					setMouseCapture(true);
				}
			}

		}
	}


	if (view.mouseOver())
	{
		if (m_child->update())
		{
			changeSize();
		}
	}
	else {
		bool capture = false;
		//枠の外のカーソルに反応しないようにする
		if (not CursorSystem::IsCaptured())
		{
			setMouseCapture(true);
			capture = true;
		}

		if (m_child->update())
		{
			changeSize();
		}

		if (capture)
		{
			setMouseCapture(false);
		}
	}

	if (isScroll() and mouseOver() and not CursorSystem::IsWheelCaptured() and Mouse::Wheel()) {
		m_value = Clamp(m_value + Mouse::Wheel() / (m_childHeight - view.h) * 10, 0.0, 1.0);
		CursorSystem::setWheelCapture(true);
		changeValueFlg = true;
	}

	if (changeValueFlg) {
		const double pos = m_value * (m_childHeight - view.h);
		m_child->setPos(view.pos + Vec2{ 0,-pos });
	}

}

void SimpleScrollbar::onDraw(const RectF&)const
{
	RectF view = getRect();
	if (isScroll())
	{
		view.w -= ScrollbarWidth;
	}

	{
		const Rect oldScissorRect = Graphics2D::GetScissorRect();
		Graphics2D::SetScissorRect(view.asRect());
		RasterizerState rs = RasterizerState::Default2D;
		rs.scissorEnable = true;
		const ScopedRenderStates2D rasterizer{ rs };
		m_child->draw(view);
		Graphics2D::SetScissorRect(oldScissorRect);
	}

	if (isScroll())
	{
		getBackBarRoundRect().draw(ColorF{ 0.9 });
		getBarRoundRect().draw(hasMouseCapture() ? ColorF{ 0.3 } : ColorF{ 0.5 });
	}
}

void SimpleScrollbar::onBuild()
{
	RectF rect = getRect();
	m_childHeight = m_child->getY(rect.w);

	if (isScroll())
	{
		m_childHeight = m_child->getY(rect.w - ScrollbarWidth);
		m_child->build({ rect.pos,rect.w - ScrollbarWidth,Max(m_childHeight,rect.h) });
	}
	else
	{
		m_child->build({ rect.pos,rect.w, Max(m_childHeight,rect.h) });
		m_value = 0;
	}

	double pos = m_value * (m_childHeight - rect.h);
	m_child->setPos(rect.pos + Vec2{ 0,-pos });
}

SizeF SimpleScrollbar::onGetSize()const
{
	return m_child->getSize();
}

double SimpleScrollbar::onGetX(double y)const
{
	return m_child->getX(y) + ScrollbarWidth;//スクロールバーの分
}

double SimpleScrollbar::onGetY(double x)const
{
	return m_child->getY(x);
}

//-----------------------------------------------
//  RectUI
//-----------------------------------------------

void RectUI::onDraw(const RectF&)const {
	getRect().draw(color);
}

SizeF RectUI::onGetSize()const {
	return m_size;
}

//-----------------------------------------------
//  TextUI
//-----------------------------------------------

void TextUI::onDraw(const RectF&)const
{
	m_font(m_text).draw(m_fontSize, getRect(), color);
}

SizeF TextUI::onGetSize()const
{
	return m_font(m_text).region(m_fontSize).size;
}

double TextUI::onGetY(double x)const
{

	const double rate = m_fontSize / (double)m_font.fontSize();

	const Array<Glyph> glyphs = m_font.getGlyphs(m_text);
	const double fontHeight = m_font.height() * rate;
	Vec2 penPos{ 0,0 };

	for (const auto& glyph : glyphs)
	{
		// 改行文字なら
		if (glyph.codePoint == U'\n')
		{
			// ペンの X 座標をリセット
			penPos.x = 0;

			// ペンの Y 座標をフォントの高さ分進める
			penPos.y += fontHeight;

			continue;
		}

		if (x < penPos.x + glyph.xAdvance * rate) {
			penPos.x = 0;
			penPos.y += fontHeight;
		}

		// ペンの X 座標を文字の幅の分進める
		penPos.x += glyph.xAdvance * rate;
	}
	return penPos.y + fontHeight;
}

ColorF TextUI::DefaultColor = Palette::Black;
AssetName TextUI::DefaultFontName;

//-----------------------------------------------
//  TextureUI
//-----------------------------------------------

bool TextureUI::onMouseOver()const
{
	const RectF rect = getRect();
	const SizeF magnification = rect.size / m_texture.size;
	const double r = Min(magnification.x, magnification.y);
	return m_texture.scaled(r).regionAt(rect.center()).mouseOver();
}

void TextureUI::onDraw(const RectF&)const
{
	const RectF rect = getRect();
	const SizeF magnification = rect.size / m_texture.size;
	const double r = Min(magnification.x, magnification.y);
	m_texture.scaled(r).drawAt(rect.center(), color);
}

SizeF TextureUI::onGetSize()const
{
	return m_texture.size;
}

//-----------------------------------------------
//  UIManager
//-----------------------------------------------

void UIManager::update(const RectF& rect)
{

	CursorSystem::SetCaptureOneFrame(false);
	CursorSystem::setWheelCapture(false);
	CursorSystem::setWheelHCapture(false);

	bool changeSizeFlg = false;

	if (m_rect != rect)
	{
		m_rect = rect;
		changeSizeFlg = true;
	}

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		auto& child = m_children[m_children.size() - 1 - i];

		if (child->update() or changeSizeFlg)
		{
			child->build(m_rect);
		}
	}
}

void UIManager::draw()const
{
	for (const auto& child : m_children)
	{
		child->draw(m_rect);
	}
}

void UIManager::setChildren(const Array<std::shared_ptr<UIElement>>& children)
{
	m_children = children;
	for (const auto& child : m_children)
	{
		child->build(m_rect);
	}
}
