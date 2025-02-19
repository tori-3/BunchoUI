#pragma once

namespace BunchoUI
{
	//-----------------------------------------------
	//  Margin
	//-----------------------------------------------

	constexpr Margin::Margin(double space)noexcept
		:Margin{ space,space } {
	}

	constexpr Margin::Margin(double vertical, double horizontal)noexcept
		:Margin{ vertical,horizontal,vertical,horizontal } {
	}

	constexpr Margin::Margin(double top, double right, double bottom, double left)noexcept
		:top{ top }, right{ right }, bottom{ bottom }, left{ left } {
	}

	constexpr SizeF Margin::getSize()const noexcept
	{
		return { getHorizontal(), getVertical() };
	}

	constexpr double Margin::getVertical()const noexcept
	{
		return top + bottom;
	}

	constexpr double Margin::getHorizontal()const noexcept
	{
		return right + left;
	}

	//-----------------------------------------------
	//  Axis
	//-----------------------------------------------

	constexpr Axis::Axis(SelfAxis selfAxis)noexcept
	{
		switch (selfAxis)
		{
		case SelfAxis::center:
			relative = 0.5;
			break;
		case SelfAxis::start:
			relative = 0.0;
			break;
		case SelfAxis::end:
			relative = 1.0;
			break;
		case SelfAxis::stretch:
			stretch = true;
			break;

		default:
			break;
		}
	}

	constexpr Axis::Axis(double relative)noexcept
		:relative{ relative } {
	}

	//-----------------------------------------------
	//  Relative
	//-----------------------------------------------

	constexpr Relative::Relative(const Axis& x, const Axis& y)noexcept
		:x{ x }, y{ y } {
	}

	inline Relative Relative::Stretch()noexcept
	{
		return { SelfAxis::stretch,SelfAxis::stretch };
	}

	inline Relative Relative::Center()noexcept
	{
		return { 0.5,0.5 };
	}

	inline Relative Relative::TopLeft()noexcept
	{
		return { 0.0,0.0 };
	}

	inline Relative Relative::Top()noexcept
	{
		return { 0.5,0.0 };
	}

	inline Relative Relative::TopRight()noexcept
	{
		return { 1.0,0.0 };
	}

	inline Relative Relative::Right()noexcept
	{
		return { 1.0,0.5 };
	}

	inline Relative Relative::BottomRight()noexcept
	{
		return { 1.0,1.0 };
	}

	inline Relative Relative::Bottom()noexcept
	{
		return { 0.5,1.0 };
	}

	inline Relative Relative::BottomLeft()noexcept
	{
		return { 0.0,1.0 };
	}

	inline Relative Relative::Left()noexcept
	{
		return { 0.0,0.5 };
	}

	//-----------------------------------------------
	//  UIElement
	//-----------------------------------------------

	inline UIElement::UIElement(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative)
		:m_margine{ margine }
		, clickable{ clickable }
		, m_relative{ relative }
		, m_flex{ flex }
		, m_width{ width }
		, m_height{ height } {
	}

	inline void UIElement::changeSize()noexcept
	{
		if (not m_changeSizeFlg)
		{
			m_changeSizeFlg = true;
			m_preSize = none;
			m_preX = none;
			m_preY = none;
		}

		UIElement* parent = getParent();
		if (parent && not parent->m_changeSizeFlg)
		{
			parent->changeSize();
		}
	}

	inline bool UIElement::isChangeSize()const noexcept
	{
		return m_changeSizeFlg;
	}

	inline void UIElement::draw(const RectF& rect)const
	{
		onDraw(rect);
	}

	inline bool UIElement::clicked()const noexcept
	{
		return m_mouseOvered && clickable && MouseL.down();
	}

	inline bool UIElement::pressed()const noexcept
	{
		return m_mouseOvered && clickable && MouseL.pressed();
	}

	inline bool UIElement::mouseOver()const noexcept
	{
		return m_mouseOvered;
	}

	inline RectF UIElement::getMargineRect()const noexcept
	{
		RectF area = m_rect;
		area.x -= m_margine.left;
		area.y -= m_margine.top;
		area.w += m_margine.getHorizontal();
		area.h += m_margine.getVertical();
		return area;
	}

	inline bool UIElement::hasMouseCapture() const noexcept
	{
		return m_hasMouseCapture;
	}

	inline void UIElement::setMouseCapture(bool captured) noexcept
	{
		if (captured != m_hasMouseCapture)
		{
			CursorSystem::SetCapture(captured);
			m_hasMouseCapture = captured;
		}
	}

	inline void UIElement::setMargine(const Margin& margin)noexcept
	{
		m_margine = margin;
		changeSize();
	}

	inline Margin UIElement::getMargin()const noexcept
	{
		return m_margine;
	}

	inline void UIElement::setWidth(const Optional<double>& width)noexcept
	{
		m_width = width;
		changeSize();
	}

	inline Optional<double>UIElement::getWidth()const noexcept
	{
		return m_width;
	}

	inline void UIElement::setHeight(const Optional<double>& height)noexcept
	{
		m_height = height;
		changeSize();
	}

	inline Optional<double>UIElement::getHeight()const noexcept
	{
		return m_height;
	}

	inline void UIElement::setFlex(double flex) noexcept
	{
		m_flex = flex;
		changeSize();
	}

	inline double UIElement::getFlex()const noexcept
	{
		return m_flex;
	}

	inline void UIElement::setRelative(const Optional<Relative>& relative)noexcept
	{
		m_relative = relative;
		changeSize();
	}

	inline Optional<Relative>UIElement::getRelative()const noexcept
	{
		return m_relative;
	}

	inline void UIElement::moveBy(const Vec2& pos)
	{
		m_rect.pos += pos;
		onMoveBy(pos);
	}

	inline void UIElement::setPos(const Vec2& pos)
	{
		Vec2 oldPos = m_rect.pos;
		oldPos.x -= m_margine.left;
		oldPos.y -= m_margine.top;
		moveBy(pos - oldPos);
	}

	inline void UIElement::setCenter(const Vec2& center)
	{
		moveBy(center - getMargineRect().center());
	}

	inline UIElement* UIElement::getParent()
	{
		return m_parent;
	}

	template<typename Type>
	Type* UIElement::getParent()
	{
		return dynamic_cast<Type*>(getParent());
	}

	inline void UIElement::setParent(UIElement* parent)
	{
		m_parent = parent;
		changeSize();
	}

	inline RectF UIElement::getRect()const noexcept
	{
		return m_rect;
	}

	inline bool UIElement::isAvailableCursor()const noexcept
	{
		return  m_isAvailableCursor;
	}

	//-----------------------------------------------
	//  NoneUI
	//-----------------------------------------------

	inline std::shared_ptr<NoneUI>NoneUI::Create()
	{
		return std::make_shared<NoneUI>();
	}

	//-----------------------------------------------
	//  ChildrenContainer
	//-----------------------------------------------

	inline ChildrenContainer::ChildrenContainer(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, const Array<std::shared_ptr<UIElement>>& children)
		: UIElement{ margine,width,height,flex,clickable,relative }
		, m_children{ children }
	{
		for (const auto& child : m_children)
		{
			child->setParent(this);
		}
	}

	inline void ChildrenContainer::setChildren(const Array<std::shared_ptr<UIElement>>& children)
	{
		m_children = children;
		for (const auto& child : m_children)
		{
			child->setParent(this);
		}
	}

	inline void ChildrenContainer::addChild(const std::shared_ptr<UIElement>& child)
	{
		child->setParent(this);
		m_children.push_back(child);
	}

	inline void ChildrenContainer::removeChild(const std::shared_ptr<UIElement>& child)
	{
		child->setParent(nullptr);
		m_children.remove(child);
		changeSize();
	}

	inline void ChildrenContainer::removeChild(const UIElement* child)
	{
		m_children.remove_if([&](const std::shared_ptr<UIElement>& value) {return value.get() == child; });
		changeSize();
	}

	inline void ChildrenContainer::removeChild(size_t index)
	{
		m_children.remove_at(index);
		changeSize();
	}

	inline void ChildrenContainer::replaceChild(const std::shared_ptr<UIElement>& oldChild, const std::shared_ptr<UIElement>& newChild)
	{
		auto it = std::find(m_children.begin(), m_children.end(), oldChild);

		if (it != m_children.end())
		{
			*it = newChild;
			newChild->setParent(this);
		}
	}

	inline void ChildrenContainer::replaceChild(UIElement* oldChild, const std::shared_ptr<UIElement>& newChild)
	{
		for (auto& child : m_children)
		{
			if(child.get()==oldChild)
			{
				child = newChild;
				newChild->setParent(this);
				break;
			}
		}
	}

	inline void ChildrenContainer::replaceChild(size_t index, const std::shared_ptr<UIElement>& newChild)
	{
		m_children[index] = newChild;
		newChild->setParent(this);
	}

	inline const Array<std::shared_ptr<UIElement>>& ChildrenContainer::getChildren()const
	{
		return m_children;
	}

	//-----------------------------------------------
	//  ChildContainer
	//-----------------------------------------------

	inline ChildContainer::ChildContainer(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, const std::shared_ptr<UIElement>& child)
		: UIElement{ margine,width,height,flex,clickable,relative }
		, m_child{ child }
	{
		m_child->setParent(this);
	}

	inline std::shared_ptr<UIElement>ChildContainer::getChild()const noexcept
	{
		return m_child;
	}

	inline void ChildContainer::setChild(const std::shared_ptr<UIElement>& child)
	{
		child->setParent(this);
		m_child = child;
	}

	inline void ChildContainer::childUpdate()
	{
		m_child->update();
	}

	inline void ChildContainer::childDraw(const RectF& drawingArea)const
	{
		m_child->draw(drawingArea);
	}

	//-----------------------------------------------
	//  PanelBase
	//-----------------------------------------------

	inline PanelBase::PanelBase(const Padding& padding, const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, const std::shared_ptr<UIElement>& child)
		: ChildContainer{ margine,width,height,flex,clickable, relative,child }
		, m_padding{ padding } {
	}

	inline Padding PanelBase::getPadding()const noexcept
	{
		return m_padding;
	}

	inline void PanelBase::setPadding(const Padding& padding) noexcept
	{
		m_padding = padding;
		changeSize();
	}

	//-----------------------------------------------
	//  BaseArrange
	//-----------------------------------------------

	template<UIDirection mainDirection, UIDirection crossDirection>
	BaseArrange<mainDirection, crossDirection>::BaseArrange(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, MainAxis mainAxis, Axis crossAxis, const Array<std::shared_ptr<UIElement>>& children)
		: ChildrenContainer{ margine,width,height,flex,clickable,relative,children }
		, m_mainAxis{ mainAxis }
		, m_crossAxis{ crossAxis } {
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	void BaseArrange<mainDirection, crossDirection>::setMainAxis(const MainAxis& mainAxis)noexcept
	{
		m_mainAxis = mainAxis;
		changeSize();
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	MainAxis BaseArrange<mainDirection, crossDirection>::getMainAxis()const noexcept
	{
		return m_mainAxis;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	void BaseArrange<mainDirection, crossDirection>::setCrossAxis(const CrossAxis& crossAxis)noexcept
	{
		m_crossAxis = crossAxis;
		changeSize();
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	CrossAxis BaseArrange<mainDirection, crossDirection>::getCrossAxis()const noexcept
	{
		return m_crossAxis;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	void BaseArrange<mainDirection, crossDirection>::onBuild()
	{

		const RectF rect = getRect();
		double pos = get<mainDirection>(rect.pos);
		double space = 0;

		const double mainPos = get<mainDirection>(rect.pos);
		const double crossPos = get<crossDirection>(rect.pos);
		const double mainLength = get<mainDirection>(rect.size);
		const double crossLength = get<crossDirection>(rect.size);

		Array<double>list(m_children.size());
		Array<size_t>expandedIndex;

		double fixedSum = 0;
		double sumRate = 0;

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i]->getFlex())
			{
				sumRate += m_children[i]->getFlex();
				expandedIndex << i;
			}
			else
			{
				list[i] = get<mainDirection>(m_children[i], crossLength);
				fixedSum += list[i];
			}
		}

		if (expandedIndex)
		{
			const double r = (mainLength - fixedSum) / sumRate;
			for (const auto i : expandedIndex)
			{
				list[i] = r * m_children[i]->getFlex();
			}
		}
		else
		{
			switch (m_mainAxis)
			{
			case MainAxis::start:
				break;
			case MainAxis::end:
				pos = get<mainDirection>(rect.br()) - list.sum();
				break;
			case MainAxis::center:
				pos = get<mainDirection>(rect.center()) - list.sum() / 2.0;
				break;
			case MainAxis::spaceAround:
				space = (mainLength - list.sum()) / m_children.size();
				pos = mainPos + space / 2.0;
				break;
			case MainAxis::spaceBetween:
				space = (mainLength - list.sum()) / (m_children.size() - 1);
				break;
			case MainAxis::spaceEvenly:
				space = (mainLength - list.sum()) / (m_children.size() + 1);
				pos = mainPos + space;
				break;
			default:
				break;
			}
		}

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			RectF childRect{};
			get<mainDirection>(childRect.pos) = pos;
			get<crossDirection>(childRect.pos) = crossPos;
			get<mainDirection>(childRect.size) = list[i];
			get<crossDirection>(childRect.size) = crossLength;

			if constexpr (mainDirection == UIDirection::x)
			{
				m_children[i]->build(childRect, Relative{ 0.5, m_crossAxis });
			}
			else
			{
				m_children[i]->build(childRect, Relative{ m_crossAxis,0.5 });
			}

			pos += list[i] + space;
		}
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	SizeF BaseArrange<mainDirection, crossDirection>::onGetSize()const 
	{
		double mainSum = 0, crossMax = 0;
		for (const auto& child : m_children)
		{
			SizeF size = child->getSize();
			mainSum += get<mainDirection>(size);
			crossMax = Max(get<crossDirection>(size), crossMax);
		}
		SizeF size{};
		get<mainDirection>(size) = mainSum;
		get<crossDirection>(size) = crossMax;
		return size;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	double BaseArrange<mainDirection, crossDirection>::onGetMain(double cross)const
	{
		double sum = 0;
		for (const auto& child : m_children)
		{
			sum += get<mainDirection>(child, cross);
		}
		return sum;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	double BaseArrange<mainDirection, crossDirection>::onGetCross(double main)const
	{
		Array<double>crossList(m_children.size());

		Array<size_t>expandedIndex;

		double fixedSum = 0;
		double sumRate = 0;

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i]->getFlex())
			{
				sumRate += m_children[i]->getFlex();
				expandedIndex << i;
			}
			else
			{
				SizeF size = m_children[i]->getSize();
				fixedSum += get<mainDirection>(size);
				crossList[i] = get<crossDirection>(size);
			}
		}

		if (expandedIndex)
		{
			double r = (main - fixedSum) / sumRate;
			for (const auto i : expandedIndex)
			{
				crossList[i] = get<crossDirection>(m_children[i], r * m_children[i]->getFlex());
			}
		}

		double max = 0;
		for (const auto& cross : crossList)
		{
			max = Max(max, cross);
		}

		return max;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	template<UIDirection direction>
	constexpr double& BaseArrange<mainDirection, crossDirection>::get(SizeF& size)
	{
		return (direction == UIDirection::x) ? size.x : size.y;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	template<UIDirection direction>
	constexpr double BaseArrange<mainDirection, crossDirection>::get(const SizeF& size)
	{
		return (direction == UIDirection::x) ? size.x : size.y;
	}

	template<UIDirection mainDirection, UIDirection crossDirection>
	template<UIDirection direction>
	constexpr double BaseArrange<mainDirection, crossDirection>::get(const std::shared_ptr<UIElement>& child, double length)
	{
		return (direction == UIDirection::x) ? child->getX(length) : child->getY(length);
	}

	//-----------------------------------------------
	//  Row
	//-----------------------------------------------

	inline Row::Row(const Parameter& para)
		:BaseArrange<UIDirection::x, UIDirection::y>{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.mainAxis,para.crossAxis, para.children } {}

	inline std::shared_ptr<Row>Row::Create(const Parameter& para)
	{
		return std::make_shared<Row>(para);
	}

	//-----------------------------------------------
	//  Column
	//-----------------------------------------------

	inline Column::Column(const Parameter& para)
		:BaseArrange<UIDirection::y, UIDirection::x>{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.mainAxis,para.crossAxis, para.children } {}

	inline std::shared_ptr<Column>Column::Create(const Parameter& para)
	{
		return std::make_shared<Column>(para);
	}

	//-----------------------------------------------
	//  StackUI
	//-----------------------------------------------

	inline StackUI::StackUI(const Parameter& para)
		:ChildrenContainer{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.children } {}

	inline std::shared_ptr<StackUI>StackUI::Create(const Parameter& para)
	{
		return std::make_shared<StackUI>(para);
	}

	//-----------------------------------------------
	//  SimpleDialog
	//-----------------------------------------------

	inline SimpleDialog::SimpleDialog(const Parameter& para)
		: ChildContainer{ {},none,none,0,false,Relative::Stretch(),para.child}
		, m_transition{ Duration{para.fadeTime},Duration{para.fadeTime} }
		, backgroundColor{ para.backgroundColor }
		, erasable{ para.erasable }
		, updateFunc{ para.updateFunc } {}

	inline std::shared_ptr<SimpleDialog>SimpleDialog::Create(const Parameter& para)
	{
		return std::make_shared<SimpleDialog>(para);
	}

	inline void SimpleDialog::close()noexcept
	{
		m_close = true;
	}

	//-----------------------------------------------
	//  RectPanel
	//-----------------------------------------------

	inline RectPanel::RectPanel(const Parameter& para)
		: PanelBase{ para.padding,para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.child }
		, r{ para.r }
		, color{ para.color } {}

	inline std::shared_ptr<RectPanel>RectPanel::Create(const Parameter& para)
	{
		return std::make_shared<RectPanel>(para);
	}

	inline RoundRect RectPanel::getRoundRect()const noexcept
	{
		return getRect().rounded(r);
	}

	//-----------------------------------------------
	//  SimpleButton
	//-----------------------------------------------

	inline SimpleButton::SimpleButton(const Parameter& para)
		: PanelBase{ para.padding,para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.child }
		, r{ para.r }
		, color{ para.color }
		, mouseOverColor{ para.mouseOverColor } {}

	inline std::shared_ptr<SimpleButton>SimpleButton::Create(const Parameter& para)
	{
		return std::make_shared<SimpleButton>(para);
	}

	inline RoundRect SimpleButton::getRoundRect()const noexcept
	{
		return getRect().rounded(r);
	}

	//-----------------------------------------------
	//  SimpleSlider
	//-----------------------------------------------

	inline SimpleSlider::SimpleSlider(const Parameter& para)
		: UIElement{ para.margine,para.width,para.height,para.flex,false,para.relative }
		, value{ para.value }
		, enabled{ para.enabled }
		, color{ para.color } {}

	inline std::shared_ptr<SimpleSlider>SimpleSlider::Create(const Parameter& para)
	{
		return std::make_shared<SimpleSlider>(para);
	}

	inline bool SimpleSlider::sliderReleased()const noexcept
	{
		return m_sliderReleased;
	}

	//-----------------------------------------------
	//  SimpleScrollbar
	//-----------------------------------------------

	inline SimpleScrollbar::SimpleScrollbar(const Parameter& para)
		: ChildContainer{ para.margine,para.width,para.height,para.flex,false,para.relative,para.child }
		,speed{para.speed}{}

	inline std::shared_ptr<SimpleScrollbar>SimpleScrollbar::Create(const Parameter& para)
	{
		return std::make_shared<SimpleScrollbar>(para);
	}

	inline bool SimpleScrollbar::isScroll()const noexcept
	{
		return getRect().h < m_childHeight;
	}

	inline double SimpleScrollbar::getRate()const noexcept
	{
		return getRect().h / m_childHeight;
	}

	inline RoundRect SimpleScrollbar::getBackBarRoundRect()const noexcept
	{
		const RectF rect = getRect();
		return RoundRect{ rect.rightX() - ScrollbarWidth,rect.y,ScrollbarWidth,rect.h,ScrollbarWidth / 2.0 };
	}

	inline RoundRect SimpleScrollbar::getBarRoundRect()const noexcept
	{
		const RectF rect = getRect();
		const double space = rect.h * (1 - getRate());
		return RoundRect{ rect.rightX() - ScrollbarWidth,rect.y + m_value * space,ScrollbarWidth,rect.h * getRate(),ScrollbarWidth / 2.0 };
	}

	//-----------------------------------------------
	//  RectUI
	//-----------------------------------------------

	inline RectUI::RectUI(const Parameter& para)
		: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative }
		, color{ para.color }
		, m_size{ para.size } {}

	inline std::shared_ptr<RectUI>RectUI::Create(const Parameter& para)
	{
		return std::make_shared<RectUI>(para);
	}

	inline SizeF RectUI::getSize()const noexcept
	{
		return m_size;
	}

	inline void RectUI::setSize(const SizeF& size)noexcept
	{
		m_size = size;
		changeSize();
	}

	//-----------------------------------------------
	//  TextUI
	//-----------------------------------------------

	inline TextUI::TextUI(const Parameter& para)
		: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative }
		, m_text{ para.text }
		, m_font{ para.font }
		, textStyle{ para.textStyle }
		, m_fontSize{ para.fontSize }
		, color{ para.color } {}

	inline std::shared_ptr<TextUI>TextUI::Create(const Parameter& para)
	{
		return std::make_shared<TextUI>(para);
	}

	inline String TextUI::getText()const noexcept
	{
		return m_text;
	}

	inline void TextUI::setText(StringView text)
	{
		m_text = text;
		changeSize();
	}

	inline Font TextUI::getFont()const noexcept
	{
		return m_font;
	}

	inline void TextUI::setFont(const Font& font)
	{
		m_font = font;
		changeSize();
	}

	inline double TextUI::getFontSize()const noexcept
	{
		return m_fontSize;
	}

	inline void TextUI::setFontSize(double fontSize)noexcept
	{
		m_fontSize = fontSize;
		changeSize();
	}

	//-----------------------------------------------
	//  TextUI
	//-----------------------------------------------

	inline TextureUI::TextureUI(const Parameter& para)
		: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative }
		, m_texture{ para.texture }
		, color{ para.color } {
	}

	inline std::shared_ptr<TextureUI>TextureUI::Create(const Parameter& para)
	{
		return std::make_shared<TextureUI>(para);
	}

	inline TextureRegion TextureUI::getTexture()const noexcept
	{
		return m_texture;
	}

	inline void TextureUI::setTexture(const TextureRegion& texture)
	{
		m_texture = texture;
		changeSize();
	}

	//-----------------------------------------------
	//  UIManager
	//-----------------------------------------------

	inline UIManager::UIManager(const RectF& rect)
		: m_rect{ rect } {}

	inline void UIManager::draw()const
	{
		if (m_stackUI->isChangeSize())
		{
			m_stackUI->build(m_rect);
		}

		m_stackUI->draw(m_rect);
	}

	inline void UIManager::setChildren(const Array<std::shared_ptr<UIElement>>& children)
	{
		m_stackUI->setChildren(children);
	}

	inline void UIManager::addChild(const std::shared_ptr<UIElement>& child)
	{
		m_stackUI->addChild(child);
	}

	inline void UIManager::removeChild(const std::shared_ptr<UIElement>& child)
	{
		m_stackUI->removeChild(child);
	}

	inline const Array<std::shared_ptr<UIElement>>& UIManager::getChildren()const noexcept
	{
		return m_stackUI->getChildren();
	}
}
