#pragma once

#include "../Base/Any.h"
#include "../Data/IDataTemplate.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Layout/Layoutable.h"

namespace Sgl
{
	class Window;

	class UIElement : public Layoutable
	{
	private:
		using UIElementEventHandler = EventHandler<UIElement>;
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseMoveEventHandler = EventHandler<UIElement, MouseMoveEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs&>;
	public:
		UIElement() = default;
		UIElement(UIElement&& other) noexcept;

		static inline const PseudoClass OnHover { "hover" };
		static inline const PseudoClass OnPressed { "pressed" };

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseMoveEventHandler> MouseMove;
		Event<MouseMoveEventHandler> MouseEnter;
		Event<MouseMoveEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;

		void SetTag(const Any& value, ValueSource source = ValueSource::Local);
		const Any& GetTag() const { return _tag; }

		void SetIsCornersRounded(bool value, ValueSource source = ValueSource::Local);
		bool GetIsCornersRounded() const { return _isCornersRounded; }

		bool IsMouseOver() const { return PseudoClasses.Has(OnHover); }	
		bool IsMousePressed() const { return PseudoClasses.Has(OnPressed); }

		static inline StyleableProperty TagProperty { &SetTag, &GetTag };
		static inline StyleableProperty IsCornersRoundedProperty { &SetIsCornersRounded, &GetIsCornersRounded };
	protected:
		void RenderBackground(RenderContext context, const FRect& rect);
		void OnCursorChanged(Cursor cursor) override;
		void OnBackgroundChanged(const Brush& background) override;
		void OnAttachedToLogicalTree() override;
		virtual void OnKeyUp(KeyEventArgs e);
		virtual void OnKeyDown(KeyEventArgs e);
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseButtonEventArgs e);
		virtual void OnMouseUp(MouseButtonEventArgs e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnMouseEnter(MouseMoveEventArgs e);
		virtual void OnMouseLeave(MouseMoveEventArgs e);
	private:
		void UpdateBackgroundRenderer(const Brush& background);
	private:
		Action<RenderContext, const FRect&> _backgroundRenderer;

		Any _tag;
		bool _isCornersRounded = false;

		ValueSource _tagSource {};	
		ValueSource _isCornersRoundedSource {};

		friend class Panel;
		friend class Window;
		friend class ContentUIElement;
	};

	class UIElementDataTemplate : public IDataTemplate
	{
	public:
		Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) override;
		bool Match(const Ref<INotifyPropertyChanged>& data) const override;
	};
}