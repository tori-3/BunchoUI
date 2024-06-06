#pragma once

namespace BunchoUI
{
	namespace CursorSystem
	{
		void SetCapture(const bool capture);

		void SetCaptureOneFrame(const bool capture);

		bool IsCaptured();

		void setWheelCapture(const bool capture);

		void setWheelHCapture(const bool capture);

		bool IsWheelCaptured();

		bool IsWheelHCaptured();

		void SetHandStyle(StringView name);

		void RequestHandStyle();
	}

	struct Margin
	{
		double top = 0.0;

		double right = 0.0;

		double bottom = 0.0;

		double left = 0.0;

		constexpr Margin()noexcept = default;

		constexpr Margin(double space)noexcept
			:Margin{ space,space } {}

		constexpr Margin(double vertical, double horizontal)noexcept
			:Margin{ vertical,horizontal,vertical,horizontal } {}

		constexpr Margin(double top, double right, double bottom, double left)noexcept
			:top{ top }, right{ right }, bottom{ bottom }, left{ left } {}

		SizeF getSize()const noexcept
		{
			return { getHorizontal(), getVertical() };
		}

		double getVertical()const noexcept
		{
			return top + bottom;
		}

		double getHorizontal()const noexcept
		{
			return right + left;
		}
	};

	using Padding = Margin;

	enum class MainAxis
	{
		start, end, center, spaceAround, spaceBetween, spaceEvenly
	};

	enum class CrossAxis
	{
		center, start, end, stretch
	};

	using SelfAxis = CrossAxis;

	struct Axis
	{
		constexpr Axis()noexcept = default;

		Axis(SelfAxis selfAxis)noexcept
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

		constexpr Axis(double relative)noexcept
			:relative{ relative } {}

		bool stretch = false;
		double relative = 0.5;
	};

	struct Relative
	{
		Axis x;
		Axis y;

		constexpr Relative(const Axis& x, const Axis& y)noexcept
			:x{ x }, y{ y } {}

		[[nodiscard]]
		static Relative Stretch()noexcept
		{
			return { SelfAxis::stretch,SelfAxis::stretch };
		}

		[[nodiscard]]
		static Relative Center()noexcept
		{
			return { 0.5,0.5 };
		}

		[[nodiscard]]
		static Relative TopLeft()noexcept
		{
			return { 0.0,0.0 };
		}

		[[nodiscard]]
		static Relative Top()noexcept
		{
			return { 0.5,0.0 };
		}

		[[nodiscard]]
		static Relative TopRight()noexcept
		{
			return { 1.0,0.0 };
		}

		[[nodiscard]]
		static Relative Right()noexcept
		{
			return { 1.0,0.5 };
		}

		[[nodiscard]]
		static Relative BottomRight()noexcept
		{
			return { 1.0,1.0 };
		}

		[[nodiscard]]
		static Relative Bottom()noexcept
		{
			return { 0.5,1.0 };
		}

		[[nodiscard]]
		static Relative BottomLeft()noexcept
		{
			return { 0.0,1.0 };
		}

		[[nodiscard]]
		static Relative Left()noexcept
		{
			return { 0.0,0.5 };
		}
	};

	class UIElement
	{
	public:
		bool clickable = false;
		String tag;

		UIElement() = default;

		UIElement(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, StringView tag)
			:m_margine{ margine }
			, clickable{ clickable }
			, m_relative{ relative }
			, m_flex{ flex }
			, m_width{ width }
			, m_height{ height }
			, tag{ tag } {}

		virtual ~UIElement();

		void changeSize()noexcept
		{
			m_changeSizeFlg = true;
		}

		//返り値はサイズが変わったかどうか
		[[nodiscard]]
		bool update();

		void draw(const RectF& rect)const
		{
			onDraw(rect);
		}

		[[nodiscard]]
		bool clicked()const noexcept
		{
			return m_mouseOvered && clickable && MouseL.down();
		}

		[[nodiscard]]
		bool pressed()const noexcept
		{
			return m_mouseOvered && clickable && MouseL.pressed();
		}

		bool mouseOver()const noexcept
		{
			return m_mouseOvered;
		}

		void build(const RectF& rect, const Relative& parentRelative = Relative::Center());

		[[nodiscard]]
		SizeF getSize()const
		{
			return getSizeWithoutMargine() + m_margine.getSize();
		}

		//y<getSize().yのときgetSize().x以上の値を返す
		//getSize().y<=y のときは基本的にgetSize().xと同値を返す
		[[nodiscard]]
		double getX(double y)const
		{
			return getXWithoutMargine(y - m_margine.getVertical()) + m_margine.getHorizontal();
		}

		//x<getSize().xのときgetSize().y以上の値を返す
		//getSize().x<=x のときは基本的にgetSize().yと同値を返す
		[[nodiscard]]
		double getY(double x)const
		{
			return getYWithoutMargine(x- m_margine.getHorizontal()) + m_margine.getVertical();
		}

		[[nodiscard]]
		RectF getMargineRect()const noexcept
		{
			RectF area = m_rect;
			area.x -= m_margine.left;
			area.y -= m_margine.top;
			area.w += m_margine.getHorizontal();
			area.h += m_margine.getVertical();
			return area;
		}

		[[nodiscard]]
		bool hasMouseCapture() const noexcept
		{
			return m_hasMouseCapture;
		}

		void setMouseCapture(bool captured) noexcept
		{
			if (captured != m_hasMouseCapture) {
				CursorSystem::SetCapture(captured);
				m_hasMouseCapture = captured;
			}
		}

		void setMargine(const Margin& margin)noexcept
		{
			m_margine = margin;
			changeSize();
		}

		[[nodiscard]]
		Margin getMargin()const noexcept
		{
			return m_margine;
		}

		void setWidth(const Optional<double>& width)noexcept
		{
			m_width = width;
			changeSize();
		}

		[[nodiscard]]
		Optional<double>getWidth()const noexcept
		{
			return m_width;
		}

		void setHeight(const Optional<double>& height)noexcept
		{
			m_height = height;
			changeSize();
		}

		[[nodiscard]]
		Optional<double>getHeight()const noexcept
		{
			return m_height;
		}

		void setFlex(double flex) noexcept
		{
			m_flex = flex;
			changeSize();
		}

		[[nodiscard]]
		double getFlex()const noexcept
		{
			return m_flex;
		}

		void setRelative(const Optional<Relative>& relative)noexcept
		{
			m_relative = relative;
			changeSize();
		}

		[[nodiscard]]
		Optional<Relative>getRelative()const noexcept
		{
			return m_relative;
		}

		//移動用の関数
		//スクロールバー内や、カーソルで移動するときなどに使う

		void moveBy(const Vec2& pos)
		{
			m_rect.pos += pos;
			onMoveBy(pos);
		}

		void setPos(const Vec2& pos)
		{
			Vec2 oldPos = m_rect.pos;
			oldPos.x -= m_margine.left;
			oldPos.y -= m_margine.top;
			moveBy(pos - oldPos);
		}

		void setCenter(const Vec2& center)
		{
			moveBy(center - getMargineRect().center());
		}

	protected:

		[[nodiscard]]
		RectF getRect()const noexcept
		{
			return m_rect;
		}

		[[nodiscard]]
		virtual SizeF onGetSize()const = 0;

		virtual void onMoveBy(const Vec2& pos);

		virtual void onUpdate();

		virtual void onDraw(const RectF& rect)const;

		virtual void onBuild();

		[[nodiscard]]
		virtual double onGetX(double y)const;

		[[nodiscard]]
		virtual double onGetY(double x)const;

		[[nodiscard]]
		virtual bool onMouseOver()const;

		[[nodiscard]]
		bool isAvailableCursor()const noexcept
		{
			return  m_isAvailableCursor;
		}

	private:
		Margin m_margine = {};
		Optional<double>m_width;
		Optional<double>m_height;
		double m_flex = 0;
		Optional<Relative>m_relative{};

		RectF m_rect{};
		bool m_changeSizeFlg = false;
		bool m_mouseOvered = false;
		bool m_hasMouseCapture = false;
		bool m_isAvailableCursor = false;

		SizeF getSizeWithoutMargine()const
		{
			if (m_width)
			{
				return SizeF{ m_width.value() ,m_height ? m_height.value(): onGetY(m_width.value()) };
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

		double getXWithoutMargine(double y)const
		{
			if (m_width)
			{
				return m_width.value();
			}
			else
			{
				return onGetX(m_height ? m_height.value() : y );
			}
		}

		double getYWithoutMargine(double x)const
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
	};

	//childが空のときに表示するUI
	class NoneUI :public UIElement
	{
	public:

		[[nodiscard]]
		static std::shared_ptr<NoneUI>Create()
		{
			return std::make_shared<NoneUI>();
		}

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;
	};

	enum class UIDirection
	{
		x, y
	};

	template<UIDirection mainDirection, UIDirection crossDirection>
	class BaseArrange :public UIElement
	{
	public:
		MainAxis m_mainAxis;
		Axis m_crossAxis;
		Array<std::shared_ptr<UIElement>>m_children;

		BaseArrange(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, StringView tag, MainAxis mainAxis, Axis crossAxis, const Array<std::shared_ptr<UIElement>>& children)
			: UIElement{ margine,width,height,flex,clickable,relative,tag }
			, m_children{ children }
			, m_mainAxis{ mainAxis }
			, m_crossAxis{ crossAxis } {}

		void setChildren(const Array<std::shared_ptr<UIElement>>& children)
		{
			m_children = children;
			changeSize();
		}

		void addChild(const std::shared_ptr<UIElement>& child)
		{
			m_children.push_back(child);
			changeSize();
		}

		void removeChild(const std::shared_ptr<UIElement>& child)
		{
			m_children.remove(child);
			changeSize();
		}

		[[nodiscard]]
		const Array<std::shared_ptr<UIElement>>& getChildren()const
		{
			return m_children;
		}

		void setMainAxis(const MainAxis& mainAxis)noexcept
		{
			m_mainAxis = mainAxis;
			changeSize();
		}

		[[nodiscard]]
		MainAxis getMainAxis()const noexcept
		{
			return m_mainAxis;
		}

		void setCrossAxis(const CrossAxis& crossAxis)noexcept
		{
			m_crossAxis = crossAxis;
			changeSize();
		}

		[[nodiscard]]
		CrossAxis getCrossAxis()const noexcept
		{
			return m_crossAxis;
		}

	protected:

		void onMoveBy(const Vec2& pos)override
		{
			for (const auto& child : m_children)
			{
				child->moveBy(pos);
			}
		}

		void onUpdate()override
		{
			for (size_t i = 0; i < m_children.size(); ++i)
			{
				if (m_children[m_children.size() - 1 - i]->update())
				{
					changeSize();
				}
			}
		}

		void onDraw(const RectF& drawingArea)const override
		{
			for (const auto& child : m_children)
			{
				if (drawingArea.intersects(child->getMargineRect()))
				{
					child->draw(drawingArea);
				}
			}
		}

		void onBuild()override
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

				if constexpr (mainDirection==UIDirection::x)
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

		//flexのときは必要ない
		SizeF onGetSize()const override
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

		double onGetMain(double cross)const
		{
			double sum = 0;
			for (const auto& child : m_children)
			{
				sum += get<mainDirection>(child, cross);
			}
			return sum;
		}

		double onGetCross(double main)const
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

	private:

		template<UIDirection direction>
		static constexpr double& get(SizeF& size)
		{
			return (direction == UIDirection::x) ? size.x : size.y;
		}

		template<UIDirection direction>
		static constexpr double get(const SizeF& size)
		{
			return (direction == UIDirection::x) ? size.x : size.y;
		}

		template<UIDirection direction>
		static constexpr double get(const std::shared_ptr<UIElement>& child, double length)
		{
			return (direction == UIDirection::x) ? child->getX(length) : child->getY(length);
		}
	};

	class Row :public BaseArrange<UIDirection::x, UIDirection::y>
	{
	public:

		struct Parameter
		{
			MainAxis mainAxis = MainAxis::start;
			Axis crossAxis = CrossAxis::start;
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative>  relative = Relative::Stretch();
			StringView tag;
			Array<std::shared_ptr<UIElement>>children;
		};

		Row(const Parameter& para)
			:BaseArrange<UIDirection::x, UIDirection::y>{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag,para.mainAxis,para.crossAxis, para.children } {}

		[[nodiscard]]
		static std::shared_ptr<Row>Create(const Parameter& para)
		{
			return std::make_shared<Row>(para);
		}

	protected:
		double onGetX(double y)const override;

		double onGetY(double x)const override;
	};

	class Column :public BaseArrange<UIDirection::y, UIDirection::x>
	{
	public:

		struct Parameter
		{
			MainAxis mainAxis = MainAxis::start;
			Axis crossAxis = CrossAxis::start;
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative>  relative = Relative::Stretch();
			StringView tag;
			Array<std::shared_ptr<UIElement>>children;
		};

		Column(const Parameter& para)
			:BaseArrange<UIDirection::y, UIDirection::x>{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag,para.mainAxis,para.crossAxis, para.children } {}

		[[nodiscard]]
		static std::shared_ptr<Column>Create(const Parameter& para)
		{
			return std::make_shared<Column>(para);
		}

	protected:

		double onGetX(double y)const override;

		double onGetY(double x)const override;
	};

	class PanelBase :public UIElement
	{
	public:

		PanelBase(const Padding& padding, const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, StringView tag, const std::shared_ptr<UIElement>& child)
			: UIElement{ margine,width,height,flex,clickable, relative,tag }
			, m_padding{ padding }
			, child{ child }
		{

		}

		Padding getPadding()const noexcept
		{
			return m_padding;
		}

		void setPadding(const Padding& padding) noexcept
		{
			m_padding = padding;
			changeSize();
		}

		[[nodiscard]]
		std::shared_ptr<UIElement>getChild()const noexcept
		{
			return child;
		}

		void setChild(const std::shared_ptr<UIElement>& ptr)
		{
			child = ptr;
			changeSize();
		}


	protected:

		std::shared_ptr<UIElement>child;

		void childUpdate()
		{
			if (child->update())
			{
				changeSize();
			}
		}

		void childDraw(const RectF& drawingArea)const
		{
			child->draw(drawingArea);
		}

		void onBuild()override;

		void onMoveBy(const Vec2& pos)override;

		SizeF onGetSize()const override;

		double onGetX(double y)const override;

		double onGetY(double x)const override;

	private:
		Padding m_padding;
	};

	class RectPanel :public PanelBase
	{
	public:
		ColorF color;

		struct Parameter
		{
			ColorF color = Palette::White;
			Padding padding = {};
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative> relative;
			StringView tag;
			std::shared_ptr<UIElement>child = NoneUI::Create();
		};

		RectPanel(const Parameter& para)
			: PanelBase{ para.padding,para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag,para.child }
			, color{ para.color } {}

		[[nodiscard]]
		static std::shared_ptr<RectPanel>Create(const Parameter& para)
		{
			return std::make_shared<RectPanel>(para);
		}

	protected:
		void onUpdate()override;

		void onDraw(const RectF& drawingArea)const override;
	};

	class SimpleButton :public PanelBase
	{
	public:
		ColorF color;
		ColorF mouseOverColor;

		struct Parameter
		{
			ColorF color = Palette::White;
			ColorF mouseOverColor = Palette::Skyblue;
			Padding padding = R;
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = true;
			Optional<Relative> relative = none;
			StringView tag;
			std::shared_ptr<UIElement>child = NoneUI::Create();
		};

		SimpleButton(const Parameter& para)
			: PanelBase{ para.padding,para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag,para.child }
			, color{ para.color }
			, mouseOverColor{ para.mouseOverColor } {}

		[[nodiscard]]
		static std::shared_ptr<SimpleButton>Create(const Parameter& para)
		{
			return std::make_shared<SimpleButton>(para);
		}

	protected:
		void onUpdate()override;

		void onDraw(const RectF& drawingArea)const override;

		bool onMouseOver()const override;

	private:
		RoundRect getRoundRect()const noexcept
		{
			return getRect().rounded(R);
		}

		static constexpr double R = 10;
	};

	class SimpleSlider :public UIElement {
	public:
		double value = 1.0;

		ColorF color;

		bool enabled;

		struct Parameter
		{
			double value = 0;
			bool enabled = true;
			ColorF color{ 0.3, 0.5, 1.0 };
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative>relative = none;
			StringView tag;
		};

		SimpleSlider(const Parameter& para)
			: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag }
			, value{ para.value }
			, enabled{ para.enabled }
			, color{ para.color } {}

		[[nodiscard]]
		static std::shared_ptr<SimpleSlider>Create(const Parameter& para)
		{
			return std::make_shared<SimpleSlider>(para);
		}

	protected:

		bool onMouseOver()const override;

		void onUpdate()override;

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

	private:
		static constexpr double BarHeight = 7;
		static constexpr double KnobR = 15;
	};

	class SimpleScrollbar :public UIElement
	{
	public:
		struct Parameter
		{
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative>relative = Relative::Stretch();
			StringView tag;
			std::shared_ptr<UIElement>child = NoneUI::Create();
		};

		SimpleScrollbar(const Parameter& para)
			: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag }
			, m_child{ para.child } {}

		[[nodiscard]]
		static std::shared_ptr<SimpleScrollbar>Create(const Parameter& para)
		{
			return std::make_shared<SimpleScrollbar>(para);
		}

		void setChild(const std::shared_ptr<UIElement>& ptr)
		{
			m_child = ptr;
			changeSize();
		}

		[[nodiscard]]
		std::shared_ptr<UIElement>getChild()const noexcept
		{
			return m_child;
		}

	protected:

		void onUpdate()override;

		void onDraw(const RectF&)const override;

		void onBuild()override;

		SizeF onGetSize()const override;

		double onGetX(double y)const override;

		double onGetY(double x)const override;

	private:

		bool isScroll()const noexcept
		{
			return getRect().h < m_childHeight;
		}

		double getRate()const noexcept
		{
			return getRect().h / m_childHeight;
		}

		RoundRect getBackBarRoundRect()const noexcept
		{
			const RectF rect = getRect();
			return RoundRect{ rect.rightX() - ScrollbarWidth,rect.y,ScrollbarWidth,rect.h,ScrollbarWidth / 2.0 };
		}

		RoundRect getBarRoundRect()const noexcept
		{
			const RectF rect = getRect();
			const double space = rect.h * (1 - getRate());
			return RoundRect{ rect.rightX() - ScrollbarWidth,rect.y + m_value * space,ScrollbarWidth,rect.h * getRate(),ScrollbarWidth / 2.0 };
		}

		std::shared_ptr<UIElement>m_child;
		double m_value = 0;
		double m_childHeight = 0;

		static constexpr double ScrollbarWidth = 20;
	};

	class RectUI :public  UIElement
	{
	public:
		ColorF color;

		struct Parameter
		{
			SizeF size = SizeF{ 50,50 };
			ColorF color = Palette::White;
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative> relative = none;
			StringView tag;
		};

		RectUI(const Parameter& para)
			: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag }
			, color{ para.color }
			, m_size{ para.size } {}

		[[nodiscard]]
		static std::shared_ptr<RectUI>Create(const Parameter& para)
		{
			return std::make_shared<RectUI>(para);
		}

		[[nodiscard]]
		SizeF getSize()const noexcept
		{
			return m_size;
		}

		void setSize(const SizeF& size)noexcept
		{
			m_size = size;
			changeSize();
		}

	protected:
		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

	private:
		SizeF m_size;
	};

	class TextUI :public UIElement
	{
	public:
		ColorF color;

		static AssetName DefaultFontName;

		static ColorF DefaultColor;

		struct Parameter
		{
			StringView text;
			Font font = DefaultFontName ? FontAsset{ TextUI::DefaultFontName } : SimpleGUI::GetFont();
			double fontSize = (double)font.fontSize();
			ColorF color = TextUI::DefaultColor;
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative> relative = none;
			StringView tag;
		};

		TextUI(const Parameter& para)
			: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag }
			, m_text{ para.text }
			, m_font{ para.font }
			, m_fontSize{ para.fontSize }
			, color{ para.color } {}

		[[nodiscard]]
		static std::shared_ptr<TextUI>Create(const Parameter& para)
		{
			return std::make_shared<TextUI>(para);
		}

		[[nodiscard]]
		String getText()const noexcept
		{
			return m_text;
		}

		void setText(StringView text)
		{
			m_text = text;
			changeSize();
		}

		[[nodiscard]]
		Font getFont()const noexcept
		{
			return m_font;
		}

		void setFont(const Font& font)
		{
			m_font = font;
			changeSize();
		}

		[[nodiscard]]
		double getFontSize()const noexcept
		{
			return m_fontSize;
		}

		void setFontSize(double fontSize)noexcept
		{
			m_fontSize = fontSize;
			changeSize();
		}

	protected:

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

		double onGetY(double x)const override;

	private:
		String m_text;
		Font m_font;
		double m_fontSize;
	};

	class TextureUI :public UIElement
	{
	public:

		ColorF color;

		struct Parameter
		{
			TextureRegion texture;
			ColorF color = Palette::White;
			Margin margine = {};
			Optional<double> width;
			Optional<double> height;
			double flex = 0;
			bool clickable = false;
			Optional<Relative>relative = none;
			StringView tag;
		};

		TextureUI(const Parameter& para)
			: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.tag }
			, m_texture{ para.texture }
			, color{ para.color } {}

		[[nodiscard]]
		static std::shared_ptr<TextureUI>Create(const Parameter& para)
		{
			return std::make_shared<TextureUI>(para);
		}

		[[nodiscard]]
		TextureRegion getTexture()const noexcept
		{
			return m_texture;
		}

		void setTexture(const TextureRegion& texture)
		{
			m_texture = texture;
			changeSize();
		}

	protected:

		bool onMouseOver()const;

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

	private:
		TextureRegion m_texture;
	};

	class UIManager {
	public:
		constexpr UIManager(const RectF& rect = Scene::Rect())noexcept
			:m_rect{ rect } {}

		void update(const RectF& rect = Scene::Rect());

		void draw()const;

		void setChildren(const Array<std::shared_ptr<UIElement>>& children);

		[[nodiscard]]
		const Array<std::shared_ptr<UIElement>>& getChildren()const noexcept
		{
			return m_children;
		}

	private:
		RectF m_rect{};
		Array<std::shared_ptr<UIElement>>m_children;
	};
}
