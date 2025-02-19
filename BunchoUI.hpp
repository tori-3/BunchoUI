#pragma once

namespace BunchoUI
{
	namespace CursorSystem
	{
		///@brief カーソルの状態(使用中かどうか)を設定する
		///@param capture カーソルの状態　true:使用中
		///@remark スライダーなどで使用する
		void SetCapture(const bool capture);

		///@brief カーソルの状態(使用中かどうか)を設定する　1フレームだけ有効
		///@param capture カーソルの状態　true:使用中
		///@remark UIの重なりを処理するために使用する
		void SetCaptureOneFrame(const bool capture);

		///@brief カーソルの状態(使用中かどうか)を取得する
		///@return カーソルの状態　true:使用中
		bool IsCaptured();

		///@brief ホイールの状態(使用中かどうか)を設定する　1フレームだけ有効
		///@param capture ホイールの状態　true:使用中
		void SetWheelCaptureOneFrame(const bool capture);

		///@brief 水平ホイールの状態(使用中かどうか)を設定する　1フレームだけ有効
		///@param capture ホイールの状態　true:使用中
		void SetWheelHCaptureOneFrame(const bool capture);

		///@brief ホイールの状態(使用中かどうか)を取得する
		///@return ホイールの状態　true:使用中
		bool IsWheelCaptured();

		///@brief 水平ホイールの状態(使用中かどうか)を取得する
		///@return 水平ホイールの状態　true:使用中
		bool IsWheelHCaptured();

		///@brief 手のアイコンのカスタムカーソルを指定する
		///@param name カスタムカーソルの名前
		///@remark ボタンに触れたときにカーソルがこのスタイルになる
		void SetHandStyle(StringView name);

		///@brief カーソルのスタイルを手のアイコンにする
		///@remark カスタムカーソルが指定されていない場合、CursorStyle::Handになる
		void RequestHandStyle();
	}

	///@brief 周りのUIとの間隔を表すクラス
	struct Margin
	{
		double top = 0.0;

		double right = 0.0;

		double bottom = 0.0;

		double left = 0.0;

		constexpr Margin()noexcept = default;

		constexpr Margin(double space)noexcept;

		constexpr Margin(double vertical, double horizontal)noexcept;

		constexpr Margin(double top, double right, double bottom, double left)noexcept;

		/// @brief スペースの合計を返す
		/// @return { 水平方向の合計 , 垂直方向の合計 }
		constexpr SizeF getSize()const noexcept;

		/// @brief 垂直方向の合計を計算
		/// @return top + bottom
		constexpr double getVertical()const noexcept;

		/// @brief 水平方向の合計を計算
		/// @return right + left
		constexpr double getHorizontal()const noexcept;
	};

	///@brief 内側のUIとの間隔を表すクラス
	using Padding = Margin;

	///@brief メインの方向(Rowなら水平)の並べ方を表す
	///@remark
	///start        [###----]
	///end          [----###]
	///center       [--###--]
	///spaceAround  [-#--#--#-]
	///spaceBetween [#--#--#]
	///spaceEvenly  [-#-#-#-]
	enum class MainAxis
	{
		start, end, center, spaceAround, spaceBetween, spaceEvenly
	};

	///@brief メインではない方向(Rowなら垂直)の並べ方を表す
	enum class CrossAxis
	{
		center, start, end, stretch
	};

	using SelfAxis = CrossAxis;

	///@brief 一つの軸の相対座標(0～1)を表すクラス
	struct Axis
	{
		constexpr Axis()noexcept = default;

		constexpr Axis(SelfAxis selfAxis)noexcept;

		constexpr Axis(double relative)noexcept;

		bool stretch = false;
		double relative = 0.5;
	};

	///@brief xとy軸の相対座標(0～1)を表すクラス
	struct Relative
	{
		Axis x;
		Axis y;

		constexpr Relative(const Axis& x, const Axis& y)noexcept;

		[[nodiscard]]
		static Relative Stretch()noexcept;

		[[nodiscard]]
		static Relative Center()noexcept;

		[[nodiscard]]
		static Relative TopLeft()noexcept;

		[[nodiscard]]
		static Relative Top()noexcept;

		[[nodiscard]]
		static Relative TopRight()noexcept;

		[[nodiscard]]
		static Relative Right()noexcept;

		[[nodiscard]]
		static Relative BottomRight()noexcept;

		[[nodiscard]]
		static Relative Bottom()noexcept;

		[[nodiscard]]
		static Relative BottomLeft()noexcept;

		[[nodiscard]]
		static Relative Left()noexcept;
	};

	///@brief すべてのUIの基底クラス
	class UIElement
	{
	public:
		/// @brief クリック可能か
		bool clickable = false;

		UIElement() = default;
		
		/// @brief コンストラクタ
		/// @param margine 周りのUIとの間隔
		/// @param width 横幅(設定しないと自動計算)
		/// @param height 縦幅(設定しないと自動計算) 
		/// @param flex RowやColumn内での比率
		/// @param clickable クリック可能か
		/// @param relative 相対座標(右寄せなどを指定)
		UIElement(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative);

		virtual ~UIElement();

		/// @brief サイズが変わったことを通知する
		void changeSize()noexcept;

		/// @brief サイズが変わったかを調べる
		/// @return サイズが変わったか
		bool isChangeSize()const noexcept;

		/// @brief UIの更新を行う
		void update();

		/// @brief UIの描画を行う
		/// @param drawArea 描画が必要な範囲
		void draw(const RectF& drawArea)const;

		/// @brief UIがクリックされたか調べる
		/// @return クリックされたか
		/// @remark clickableがfalseのときは常にfalse
		[[nodiscard]]
		bool clicked()const noexcept;

		/// @brief UIが押されているか調べる
		/// @return 押されているか
		/// @remark clickableがfalseのときは常にfalse
		[[nodiscard]]
		bool pressed()const noexcept;

		/// @brief UIにマウスが触れているか調べる
		/// @return 触れているか
		bool mouseOver()const noexcept;

		/// @brief UIを再配置する
		/// @param rect 配置する長方形
		/// @param parentRelative relativeが決まっていないときの相対座標
		void build(const RectF& rect, const Relative& parentRelative = Relative::Center());

		/// @brief UIのデフォルトの大きさを計算する
		/// @return デフォルトの大きさ
		/// @remark マージンを含む
		[[nodiscard]]
		SizeF getSize();

		/// @brief 縦幅が決まっているときの横幅を計算する
		/// @param x 縦幅
		/// @return 横幅
		/// @remark マージンを含む
		[[nodiscard]]
		double getX(double y);

		/// @brief 横幅が決まっているときの縦幅を計算する
		/// @param x 横幅
		/// @return 縦幅
		/// @remark マージンを含む
		[[nodiscard]]
		double getY(double x);

		/// @brief マージン(周りの空間)を含む長方形を計算する
		/// @return マージンを含む長方形
		[[nodiscard]]
		RectF getMargineRect()const noexcept;

		/// @brief 自身がカーソルを使用中か調べる
		/// @return カーソルを使用中か(true:使用中)
		[[nodiscard]]
		bool hasMouseCapture() const noexcept;

		/// @brief カーソルの状態(使用中か)を変更する
		/// @param captured カーソルの状態(true:使用中)
		void setMouseCapture(bool captured) noexcept;

		/// @brief UIを移動する
		/// @param pos 動かす座標
		void moveBy(const Vec2& pos);

		/// @brief UIを移動する
		/// @param pos 移動先の座標
		void setPos(const Vec2& pos);

		/// @brief UIを移動する
		/// @param center 移動先の中心座標
		void setCenter(const Vec2& center);

		/// @brief 親のUIを取得する
		/// @return 親のUI 
		UIElement* getParent();

		/// @brief 親のUIを取得する
		/// @tparam Type 親のUIの型
		/// @return 親のUI 
		template<typename Type>
		Type* getParent();

		void setMargine(const Margin& margin)noexcept;

		[[nodiscard]]
		Margin getMargin()const noexcept;

		void setWidth(const Optional<double>& width)noexcept;

		[[nodiscard]]
		Optional<double>getWidth()const noexcept;

		void setHeight(const Optional<double>& height)noexcept;

		[[nodiscard]]
		Optional<double>getHeight()const noexcept;

		void setFlex(double flex) noexcept;

		[[nodiscard]]
		double getFlex()const noexcept;

		void setRelative(const Optional<Relative>& relative)noexcept;

		[[nodiscard]]
		Optional<Relative>getRelative()const noexcept;

		void setParent(UIElement* parent);

	protected:

		/// @brief UIに使用できる長方形を取得する
		/// @return UIに使用できる長方形
		[[nodiscard]]
		RectF getRect()const noexcept;

		/// @brief UIのデフォルトの大きさを計算する
		/// @return デフォルトの大きさ
		[[nodiscard]]
		virtual SizeF onGetSize()const = 0;

		/// @brief UIの座標が移動されたときの処理
		virtual void onMoveBy(const Vec2& pos);

		/// @brief UIの更新処理
		virtual void onUpdate();

		/// @brief UIの描画処理
		/// @param drawArea 描画範囲
		virtual void onDraw(const RectF& drawArea)const;

		/// @brief 再レイアウト時の処理
		virtual void onBuild();

		/// @brief 縦幅が決まっているときの横幅を計算する
		/// @param x 縦幅
		/// @return 横幅
		[[nodiscard]]
		virtual double onGetX(double y)const;

		/// @brief 横幅が決まっているときの縦幅を計算する
		/// @param x 横幅
		/// @return 縦幅
		[[nodiscard]]
		virtual double onGetY(double x)const;

		/// @brief カーソルの座標とUIが重なっているか調べる
		/// @return 重なっているか
		[[nodiscard]]
		virtual bool onMouseOver()const;

		/// @brief カーソルが使用可能か調べる
		/// @return 誰も使用してない or 自身が使用中ならtrue
		[[nodiscard]]
		bool isAvailableCursor()const noexcept;

	private:
		Margin m_margine;
		Optional<double>m_width;
		Optional<double>m_height;
		double m_flex = 0;
		Optional<Relative>m_relative;

		UIElement* m_parent = nullptr;

		RectF m_rect{};
		bool m_changeSizeFlg = true;
		bool m_mouseOvered = false;
		bool m_hasMouseCapture = false;
		bool m_isAvailableCursor = false;

		Optional<SizeF>m_preX;
		Optional<SizeF>m_preY;
		Optional<SizeF>m_preSize;

		SizeF getSizeWithoutMargine()const;

		double getXWithoutMargine(double y)const;

		double getYWithoutMargine(double x)const;
	};

	///@brief childが空のときに表示するUI
	class NoneUI :public UIElement
	{
	public:

		[[nodiscard]]
		static std::shared_ptr<NoneUI>Create();

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;
	};

	/// @brief 複数の子供を持つクラスの基底クラス
	class ChildrenContainer:public UIElement
	{
	public:
		ChildrenContainer(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, const Array<std::shared_ptr<UIElement>>& children);

		void setChildren(const Array<std::shared_ptr<UIElement>>& children);

		void addChild(const std::shared_ptr<UIElement>& child);

		void removeChild(const std::shared_ptr<UIElement>& child);

		void removeChild(const UIElement* child);

		void removeChild(size_t index);

		void replaceChild(const std::shared_ptr<UIElement>& oldChild, const std::shared_ptr<UIElement>& newChild);

		void replaceChild(UIElement* child, const std::shared_ptr<UIElement>& newChild);

		void replaceChild(size_t index, const std::shared_ptr<UIElement>& newChild);

		[[nodiscard]]
		const Array<std::shared_ptr<UIElement>>& getChildren()const;

	protected:

		void onMoveBy(const Vec2& pos)override;

		void onUpdate()override;

		void onDraw(const RectF& drawingArea)const override;

		Array<std::shared_ptr<UIElement>>m_children;
	};

	/// @brief 単一の子供を持つクラスの基底クラス
	class ChildContainer :public UIElement
	{
	public:
		ChildContainer(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, const std::shared_ptr<UIElement>& child);

		[[nodiscard]]
		std::shared_ptr<UIElement>getChild()const noexcept;

		void setChild(const std::shared_ptr<UIElement>& child);

	protected:

		std::shared_ptr<UIElement>m_child;

		void onBuild()override;

		SizeF onGetSize()const override;

		double onGetX(double y)const override;

		double onGetY(double x)const override;

		void onMoveBy(const Vec2& pos)override;

		void childUpdate();

		void childDraw(const RectF& drawingArea)const;
	};

	/// @brief 単一の子供と、子供に対するPaddingを持つクラスの基底クラス
	class PanelBase :public ChildContainer
	{
	public:

		PanelBase(const Padding& padding, const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, const std::shared_ptr<UIElement>& child);

		Padding getPadding()const noexcept;

		void setPadding(const Padding& padding) noexcept;

	protected:

		void onBuild()override;

		SizeF onGetSize()const override;

		double onGetX(double y)const override;

		double onGetY(double x)const override;

	private:
		Padding m_padding;
	};

	enum class UIDirection
	{
		x, y
	};

	/// @brief RowとColumnの基底クラス
	/// @tparam mainDirection メインの軸
	/// @tparam crossDirection メインにクロスする軸
	template<UIDirection mainDirection, UIDirection crossDirection>
	class BaseArrange :public ChildrenContainer
	{
	public:
		MainAxis m_mainAxis;
		Axis m_crossAxis;

		BaseArrange(const Margin& margine, const Optional<double>& width, const Optional<double>& height, double flex, bool clickable, const Optional<Relative>& relative, MainAxis mainAxis, Axis crossAxis, const Array<std::shared_ptr<UIElement>>& children);

		void setMainAxis(const MainAxis& mainAxis)noexcept;

		[[nodiscard]]
		MainAxis getMainAxis()const noexcept;

		void setCrossAxis(const CrossAxis& crossAxis)noexcept;

		[[nodiscard]]
		CrossAxis getCrossAxis()const noexcept;

	protected:

		void onBuild()override;

		SizeF onGetSize()const override;

		double onGetMain(double cross)const;

		double onGetCross(double main)const;

	private:

		template<UIDirection direction>
		static constexpr double& get(SizeF& size);

		template<UIDirection direction>
		static constexpr double get(const SizeF& size);

		template<UIDirection direction>
		static constexpr double get(const std::shared_ptr<UIElement>& child, double length);
	};

	/// @brief 横に並べるUI
	class Row :public BaseArrange<UIDirection::x, UIDirection::y>
	{
	public:

		struct Parameter
		{
			MainAxis mainAxis = MainAxis::center;/// @brief 横方向の並べ方
			Axis crossAxis = CrossAxis::center;/// @brief 縦方向の並べ方
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative> relative = Relative::Stretch();/// @brief 相対座標(右寄せ、中央寄せなどを指定)
			Array<std::shared_ptr<UIElement>> children;/// @brief 並べるUIたち
		};

		Row(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<Row>Create(const Parameter& para);

	protected:
		double onGetX(double y)const override;

		double onGetY(double x)const override;
	};

	/// @brief 縦に並べるUI
	class Column :public BaseArrange<UIDirection::y, UIDirection::x>
	{
	public:

		struct Parameter
		{
			MainAxis mainAxis = MainAxis::center;/// @brief 縦方向の並べ方
			Axis crossAxis = CrossAxis::center;/// @brief 横方向の並べ方
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative> relative = Relative::Stretch();/// @brief 相対座標(右寄せ、中央寄せなどを指定)
			Array<std::shared_ptr<UIElement>>children;/// @brief 並べるUIたち
		};

		Column(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<Column>Create(const Parameter& para);

	protected:

		double onGetX(double y)const override;

		double onGetY(double x)const override;
	};

	/// @brief 上に重ねるUI
	class StackUI :public ChildrenContainer
	{
	public:

		struct Parameter
		{
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative> relative = Relative::Stretch();/// @brief 相対座標(右寄せ、中央寄せなどを指定)
			Array<std::shared_ptr<UIElement>>children;/// @brief 並べるUIたち
		};

		StackUI(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<StackUI>Create(const Parameter& para);

	protected:

		SizeF onGetSize()const override;

		double onGetX(double y)const override;

		double onGetY(double x)const override;

		void onBuild()override;
	};

	/// @brief シンプルなダイアログ
	/// @remark 基本、StackUIの子供として使う
	class SimpleDialog :public ChildContainer
	{
	public:

		/// @brief 毎フレーム実行される関数
		std::function<void(SimpleDialog*)>updateFunc;

		/// @brief ダイアログの背景の色
		ColorF backgroundColor{ 0,0.3 };

		/// @brief ダイアログ以外をクリックしたら消えるか
		bool erasable;

		struct Parameter
		{
			ColorF backgroundColor{ 0,0.5 };/// @brief ダイアログの背景の色
			bool erasable = true;/// @brief ダイアログ以外をクリックしたら消えるか
			double fadeTime = 0.2;/// @brief フェードイン・アウトの時間
			std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief ダイアログのUI
			std::function<void(SimpleDialog*)>updateFunc = nullptr;/// @brief 毎フレーム実行される関数
		};

		SimpleDialog(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<SimpleDialog>Create(const Parameter& para);

		/// @brief ダイアログを閉じる
		/// @remark 親がChildrenContainerの派生クラスでないと消せない
		void close()noexcept;

	protected:
		void onUpdate()override;

		void onDraw(const RectF& drawArea)const override;

	private:
		bool m_close = false;
		Transition m_transition;

	};

	/// @brief シンプルな長方形のパネル
	class RectPanel :public PanelBase
	{
	public:

		/// @brief 角の丸み
		double r;

		// @brief 長方形の色
		ColorF color;

		struct Parameter
		{
			double r = 0;/// @brief 角の丸み
			ColorF color = Palette::White;/// @brief 長方形の色
			Padding padding = r - r * Math::InvSqrt2;/// @brief 内側のUIとの間隔
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative> relative;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
			std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief 子供のUI
		};

		RectPanel(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<RectPanel>Create(const Parameter& para);

		bool onMouseOver()const override;

	protected:

		void onUpdate()override;

		void onDraw(const RectF& drawingArea)const override;

	private:

		RoundRect getRoundRect()const noexcept;
	};

	/// @brief シンプルなボタン
	class SimpleButton :public PanelBase
	{
	public:

		/// @brief 角の丸み
		double r;

		/// @brief ボタンの色
		ColorF color;

		/// @brief マウスが触れているときの色
		ColorF mouseOverColor;

		struct Parameter
		{
			double r = 10;
			ColorF color = Palette::White;/// @brief ボタンの色
			ColorF mouseOverColor = Palette::Skyblue;/// @brief マウスが触れているときの色
			Padding padding = r - r * Math::InvSqrt2;/// @brief 内側のUIとの間隔
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = true;/// @brief クリック可能か
			Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
			std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief 子供のUI
		};

		SimpleButton(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<SimpleButton>Create(const Parameter& para);

	protected:

		void onUpdate()override;

		void onDraw(const RectF& drawingArea)const override;

		bool onMouseOver()const override;

	private:

		RoundRect getRoundRect()const noexcept;
	};

	/// @brief シンプルなスライダー
	class SimpleSlider :public UIElement {
	public:
		/// @brief スライダーの値
		double value = 1.0;

		/// @brief スライダーの色
		ColorF color;

		/// @brief スライダーが有効か
		bool enabled;

		struct Parameter
		{
			double value = 0;/// @brief スライダーの値
			bool enabled = true;/// @brief スライダーが有効か
			ColorF color{ 0.3, 0.5, 1.0 };/// @brief スライダーの色
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			Optional<Relative>relative;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		};

		SimpleSlider(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<SimpleSlider>Create(const Parameter& para);

		/// @brief スライダーが離されたかを調べる
		/// @return スライダーが離されたか
		bool sliderReleased()const noexcept;

	protected:

		bool onMouseOver()const override;

		void onUpdate()override;

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

	private:

		bool m_sliderReleased = false;

		static constexpr double BarHeight = 7;
		static constexpr double KnobR = 15;
	};

	/// @brief シンプルなスクロールバー
	class SimpleScrollbar :public ChildContainer
	{
	public:

		/// @brief ホイールのスピード
		double speed;

		struct Parameter
		{
			double speed = 20;/// @brief ホイールのスピード
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			Optional<Relative>relative = Relative::Stretch();/// @brief 相対座標(右寄せ、中央寄せなどを指定)
			std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief 子供のUI
		};

		SimpleScrollbar(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<SimpleScrollbar>Create(const Parameter& para);

	protected:

		void onUpdate()override;

		void onDraw(const RectF&)const override;

		void onBuild()override;

		double onGetX(double y)const override;

	private:

		bool isScroll()const noexcept;

		double getRate()const noexcept;

		RoundRect getBackBarRoundRect()const noexcept;

		RoundRect getBarRoundRect()const noexcept;

		double m_value = 0;
		double m_childHeight = 0;

		static constexpr double ScrollbarWidth = 20;
	};

	/// @brief 長方形を表示するのUI
	class RectUI :public  UIElement
	{
	public:

		/// @brief 長方形の色
		ColorF color;

		struct Parameter
		{
			SizeF size = SizeF{ 50,50 };/// @brief 長方形のサイズ
			ColorF color = Palette::White;/// @brief 長方形の色
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		};

		RectUI(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<RectUI>Create(const Parameter& para);

		[[nodiscard]]
		SizeF getSize()const noexcept;

		void setSize(const SizeF& size)noexcept;

	protected:
		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

	private:
		SizeF m_size;
	};

	/// @brief 文字を表示するUI
	class TextUI :public UIElement
	{
	public:

		/// @brief 文字のスタイル
		TextStyle textStyle;

		/// @brief 文字の色
		ColorF color;

		/// @brief デフォルトのフォントアセット名
		static AssetName DefaultFontName;

		/// @brief デフォルトの文字の色
		static ColorF DefaultColor;

		struct Parameter
		{
			StringView text;/// @brief 表示する文字列
			Font font = DefaultFontName ? FontAsset{ TextUI::DefaultFontName } : SimpleGUI::GetFont();/// @brief フォント
			TextStyle textStyle;/// @brief 文字のスタイル
			double fontSize = (double)font.fontSize();/// @brief 文字の大きさ
			ColorF color = TextUI::DefaultColor;/// @brief 文字の色
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		};

		TextUI(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<TextUI>Create(const Parameter& para);

		[[nodiscard]]
		String getText()const noexcept;

		void setText(StringView text);

		[[nodiscard]]
		Font getFont()const noexcept;

		void setFont(const Font& font);

		[[nodiscard]]
		double getFontSize()const noexcept;

		void setFontSize(double fontSize)noexcept;

	protected:

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

		double onGetY(double x)const override;

	private:
		String m_text;
		Font m_font;
		double m_fontSize;
	};

	/// @brief 画像を表示するUI
	class TextureUI :public UIElement
	{
	public:

		/// @brief 画像の色
		ColorF color;

		struct Parameter
		{
			TextureRegion texture;/// @brief 表示する画像
			ColorF color = Palette::White;/// @brief 画像の色
			Margin margine;/// @brief 周りのUIとの間隔
			Optional<double> width;/// @brief 横幅(設定しないと自動計算)
			Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
			double flex = 0;/// @brief RowやColumn内での比率
			bool clickable = false;/// @brief クリック可能か
			Optional<Relative>relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		};

		TextureUI(const Parameter& para);

		[[nodiscard]]
		static std::shared_ptr<TextureUI>Create(const Parameter& para);

		[[nodiscard]]
		TextureRegion getTexture()const noexcept;

		void setTexture(const TextureRegion& texture);

	protected:

		bool onMouseOver()const;

		void onDraw(const RectF&)const override;

		SizeF onGetSize()const override;

	private:
		TextureRegion m_texture;
	};

	/// @brief UIを管理するクラス
	class UIManager
	{
	public:

		/// @brief コンストラクタ
		/// @param rect UIを配置する領域
		explicit UIManager(const RectF& rect = Scene::Rect());

		/// @brief 更新する
		/// @param rect UIを配置する領域
		void update(const RectF& rect = Scene::Rect());

		/// @brief レイアウトの更新と描画を行う
		void draw()const;

		void setChildren(const Array<std::shared_ptr<UIElement>>& children);

		void addChild(const std::shared_ptr<UIElement>& child);

		void removeChild(const std::shared_ptr<UIElement>& child);

		[[nodiscard]]
		const Array<std::shared_ptr<UIElement>>& getChildren()const noexcept;

	private:
		RectF m_rect{};
		std::shared_ptr<StackUI>m_stackUI = StackUI::Create({});
	};
}

#include "BunchoUI.ipp"
