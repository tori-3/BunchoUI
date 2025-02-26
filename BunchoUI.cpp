#include"BunchoUI.hpp"

namespace BunchoUI
{
	namespace
	{
		bool g_mouseCaptured = false;

		bool g_mouseCapturedOneFrame = false;

		bool g_wheelCapturedOneFrame = false;

		bool g_wheelHCapturedOneFrame = false;

		String g_handStyleName;
	}

	//-----------------------------------------------
	//  CursorSystem
	//-----------------------------------------------
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
		return g_mouseCaptured || g_mouseCapturedOneFrame;
	}

	void CursorSystem::SetWheelCaptureOneFrame(const bool capture)
	{
		g_wheelCapturedOneFrame = capture;
	}

	void CursorSystem::SetWheelHCaptureOneFrame(const bool capture)
	{
		g_wheelHCapturedOneFrame = capture;
	}

	bool CursorSystem::IsWheelCaptured()
	{
		return g_wheelCapturedOneFrame;
	}

	bool CursorSystem::IsWheelHCaptured()
	{
		return g_wheelHCapturedOneFrame;
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

	void UIElement::update()
	{
		m_isAvailableCursor = (not CursorSystem::IsCaptured()) || hasMouseCapture();

		m_mouseOvered = m_isAvailableCursor && onMouseOver();

		if (m_mouseOvered && clickable)
		{
			CursorSystem::RequestHandStyle();
		}

		onUpdate();

		if (m_mouseOvered)
		{
			CursorSystem::SetCaptureOneFrame(true);
		}
	};

	void UIElement::build(const RectF& rect, const Relative& parentRelative)
	{
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

		SizeF size;

		if (relative.x.stretch && relative.y.stretch)
		{
			size = area.size;
		}
		else
		{
			size = getSizeWithoutMargine();

			if (area.w < size.x)
			{
				if (area.h < size.y)
				{
					size = area.size;
				}
				else
				{
					size.x = area.w;
					if (relative.y.stretch)
					{
						size.y = area.h;
					}
					else
					{
						size.y = getYWithoutMargine(area.w);
					}
				}
			}
			else if (area.h < size.y)
			{
				size.y = area.h;

				if (relative.x.stretch)
				{
					size.x = area.w;
				}
				else
				{
					size.x = getXWithoutMargine(area.h);
				}
			}
			else
			{
				if (relative.x.stretch)
				{
					size.x = area.w;
				}

				if (relative.y.stretch)
				{
					size.y = area.h;
				}
			}
		}

		const RectF newRect
		{
			area.x + (area.w - size.x) * relative.x.relative,
			area.y + (area.h - size.y) * relative.y.relative,
			size
		};

		if (m_changeSizeFlg)
		{
			m_changeSizeFlg = false;
		}
		else if (m_rect.size == newRect.size)
		{
			if (m_rect.pos != newRect.pos)
			{
				moveBy(newRect.pos - m_rect.pos);
			}
			return;
		}

		m_rect = newRect;
		onBuild();
	}

	void UIElement::onMoveBy(const Vec2&)
	{
		onBuild();
	}

	void UIElement::onUpdate() {}

	void UIElement::onDraw(const RectF&)const {}

	void UIElement::onBuild() {}

	double UIElement::onGetX(double)const
	{
		return onGetSize().x;
	}

	double UIElement::onGetY(double)const
	{
		return onGetSize().y;
	}

	bool UIElement::onMouseOver()const
	{
		return getRect().mouseOver();
	}

	SizeF UIElement::getSize()
	{
		if (not m_preSize)
		{
			m_preSize = getSizeWithoutMargine() + m_margine.getSize();
		}
		return *m_preSize;
	}

	double UIElement::getX(double y)
	{
		if (not m_preX)
		{
			m_preX = { getXWithoutMargine(y - m_margine.getVertical()) + m_margine.getHorizontal() ,y };
		}
		else if (m_preX->y != y)
		{
			m_preX = { getXWithoutMargine(y - m_margine.getVertical()) + m_margine.getHorizontal() ,y };
		}
		return m_preX->x;
	}

	double UIElement::getY(double x)
	{
		if (not m_preY)
		{
			m_preY = { x,getYWithoutMargine(x - m_margine.getHorizontal()) + m_margine.getVertical() };
		}
		else if (m_preY->x != x)
		{
			m_preY = { x,getYWithoutMargine(x - m_margine.getHorizontal()) + m_margine.getVertical() };
		}
		return m_preY->y;
	}

	SizeF UIElement::getSizeWithoutMargine()const
	{
		if (m_width)
		{
			return SizeF{ m_width.value() ,m_height ? m_height.value() : onGetY(m_width.value()) };
		}
		else if (m_height)
		{
			return SizeF{ onGetX(m_height.value()),m_height.value() };
		}
		else
		{
			return onGetSize();
		}
	}

	double UIElement::getXWithoutMargine(double y)const
	{
		if (m_width)
		{
			return m_width.value();
		}
		else
		{
			return onGetX(m_height ? m_height.value() : y);
		}
	}

	double UIElement::getYWithoutMargine(double x)const
	{
		if (m_height)
		{
			return m_height.value();
		}
		else
		{
			return onGetY(m_width ? m_width.value() : x);
		}
	}


	////-----------------------------------------------
	////  NoneUI
	////-----------------------------------------------

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
	//  ChildrenContainer
	//-----------------------------------------------

	void ChildrenContainer::onMoveBy(const Vec2& pos)
	{
		for (const auto& child : m_children)
		{
			child->moveBy(pos);
		}
	}

	void ChildrenContainer::onUpdate()
	{
		for (size_t i = 0; i < m_children.size(); ++i)
		{
			m_children[m_children.size() - 1 - i]->update();
		}
	}

	void ChildrenContainer::onDraw(const RectF& drawingArea)const
	{
		for (const auto& child : m_children)
		{
			if (drawingArea.intersects(child->getMargineRect()))
			{
				child->draw(drawingArea);
			}
		}
	}

	//-----------------------------------------------
	//  ChildContainer
	//-----------------------------------------------

	void ChildContainer::onBuild()
	{
		m_child->build(getRect());
	}

	SizeF ChildContainer::onGetSize()const
	{
		return m_child->getSize();
	}

	double ChildContainer::onGetX(double y)const
	{
		return m_child->getX(y);
	}

	double ChildContainer::onGetY(double x)const
	{
		return m_child->getY(x);
	}

	void ChildContainer::onMoveBy(const Vec2& pos)
	{
		m_child->moveBy(pos);
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
		m_child->build(area);
	}

	SizeF PanelBase::onGetSize()const
	{
		return m_child->getSize() + m_padding.getSize();
	}

	double PanelBase::onGetX(double y)const
	{
		return m_child->getX(y - m_padding.getVertical()) + m_padding.getHorizontal();
	}

	double PanelBase::onGetY(double x)const
	{
		return m_child->getY(x - m_padding.getHorizontal()) + m_padding.getVertical();
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
	//  StackUI
	//-----------------------------------------------

	SizeF StackUI::onGetSize()const
	{
		SizeF max{};
		for (auto& child : m_children)
		{
			const SizeF size = child->getSize();
			max.x = Max(max.x, size.x);
			max.y = Max(max.y, size.y);
		}
		return max;
	}

	double StackUI::onGetX(double y)const
	{
		double max = 0;
		for (auto& child : m_children)
		{
			max = Max(max, child->getX(y));
		}
		return max;
	}

	double StackUI::onGetY(double x)const
	{
		double max = 0;
		for (auto& child : m_children)
		{
			max = Max(max, child->getY(x));
		}
		return max;
	}

	void StackUI::onBuild()
	{
		for (auto& child : m_children)
		{
			child->build(getRect());
		}
	}

	//-----------------------------------------------
	//  SimpleDialog
	//-----------------------------------------------

	void SimpleDialog::onUpdate()
	{
		m_transition.update(not m_close);

		if (m_transition.isOne())
		{
			childUpdate();
			if (updateFunc)
			{
				updateFunc(this);
			}

			if (erasable && not m_child->mouseOver() && MouseL.down())
			{
				close();
			}
		}

		if (m_close && m_transition.isZero())
		{
			if (auto ptr = getParent<ChildrenContainer>())
			{
				ptr->removeChild(this);
			}
		}
	}

	void SimpleDialog::onDraw(const RectF& drawArea)const
	{
		ScopedColorMul2D mul{ AlphaF(m_transition.value()) };
		getRect().draw(backgroundColor);
		childDraw(drawArea);
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
		getRoundRect().draw(color);
		childDraw(drawingArea);
	}

	bool RectPanel::onMouseOver()const
	{
		return getRoundRect().mouseOver();
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
		const Transformer2D transform{ pressed() ? Mat3x2::Scale(0.95,getRect().center()) : Mat3x2::Identity() };
		getRoundRect().draw((mouseOver() && clickable) ? mouseOverColor : color);
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

	bool SimpleSlider::onMouseOver()const
	{
		const RectF rect = getRect();
		return RectF{ Arg::center = rect.center(),rect.w, KnobR * 2 }.mouseOver();
	}

	void SimpleSlider::onUpdate()
	{
		m_sliderReleased = false;

		if (enabled && isAvailableCursor())
		{
			const RectF rect = getRect();
			const RectF hitBox{ Arg::center = rect.center(),rect.w, KnobR * 2 };

			if (hasMouseCapture())
			{
				CursorSystem::RequestHandStyle();
				const double w = rect.w - KnobR * 2;
				value = Clamp(Cursor::PosF().x - (rect.x + KnobR), 0.0, w) / w;
				if (MouseL.up())
				{
					m_sliderReleased = true;
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
		RectF{ Arg::center = rect.leftCenter() + Vec2{length / 2 + KnobR,0},length,BarHeight }.draw(color);
		if (not enabled)
		{
			RectF{ Arg::center = rect.center(),w,BarHeight }.draw(ColorF{ 0,0.4 });
		}
		Circle{ length + rect.x + KnobR, rect.center().y ,KnobR }.draw(enabled ? Palette::White : Palette::Lightgray);
		if (not enabled)
		{
			Circle{ length + rect.x + KnobR, rect.center().y ,KnobR }.draw(ColorF{ 0,0.4 });
		}
	}

	SizeF SimpleSlider::onGetSize()const
	{
		return { 150 + KnobR * 2,KnobR * 2 };
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
			childUpdate();
		}
		else {
			bool capture = false;
			//枠の外のカーソルに反応しないようにする
			if (not CursorSystem::IsCaptured())
			{
				setMouseCapture(true);
				capture = true;
			}

			childUpdate();

			if (capture)
			{
				setMouseCapture(false);
			}
		}

		if (isScroll() && mouseOver() && not CursorSystem::IsWheelCaptured() && Mouse::Wheel()) {
			m_value = Clamp(m_value + Mouse::Wheel() / (m_childHeight - view.h) * speed, 0.0, 1.0);
			CursorSystem::SetWheelCaptureOneFrame(true);
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

	double SimpleScrollbar::onGetX(double y)const
	{
		return m_child->getX(y) + ScrollbarWidth;//スクロールバーの分
	}

	//-----------------------------------------------
	//  RectUI
	//-----------------------------------------------

	void RectUI::onDraw(const RectF&)const
	{
		getRect().draw(color);
	}

	SizeF RectUI::onGetSize()const
	{
		return m_size;
	}

	//-----------------------------------------------
	//  TextUI
	//-----------------------------------------------

	void TextUI::onDraw(const RectF&)const
	{
		RectF rect = getRect();
		//https://github.com/Siv3D/siv8/issues/57 の対策
		rect.w = Max(rect.w + 0.1, 42.5);
		m_font(m_text).draw(textStyle, m_fontSize, rect, color);
	}

	SizeF TextUI::onGetSize()const
	{
		return m_font(m_text).region(m_fontSize).size;
	}

	double TextUI::onGetY(double x)const
	{
		const double scale = m_fontSize / (double)m_font.fontSize();
		const Array<Glyph> glyphs = m_font.getGlyphs(m_text);
		const double fontHeight = m_font.height() * scale;
		Vec2 penPos{ 0,0 };

		for (const auto& glyph : glyphs)
		{
			if (glyph.codePoint == U'\n')
			{
				penPos.x = 0;
				penPos.y += fontHeight;
				continue;
			}

			if (x < penPos.x + glyph.xAdvance * scale)
			{
				penPos.x = 0;
				penPos.y += fontHeight;
			}

			penPos.x += glyph.xAdvance * scale;
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
		CursorSystem::SetWheelCaptureOneFrame(false);
		CursorSystem::SetWheelHCaptureOneFrame(false);

		bool changeSizeFlg = false;

		m_stackUI->update();

		if (m_rect != rect)
		{
			m_rect = rect;
			m_stackUI->changeSize();
		}
	}
}
