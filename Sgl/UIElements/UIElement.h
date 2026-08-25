#pragma once

#include "../Base/Any.h"
#include "../Data/IDataTemplate.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Layout/Layoutable.h"
#include "../Render/RenderFragment.h"

namespace Sgl
{
	class Window;

	class UIElement : public Layoutable
	{
	private:
		using UIElementEventHandler = EventHandler<UIElement>;
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseMoveEventHandler = EventHandler<UIElement, MouseMoveEventArgs>;
		using MouseClickEventHandler = EventHandler<UIElement, MouseClickEventArgs&>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs&>;
	public:
		UIElement() = default;
		UIElement(UIElement&& other) noexcept;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseMoveEventHandler> MouseMove;
		Event<MouseMoveEventHandler> MouseEnter;
		Event<MouseMoveEventHandler> MouseLeave;
		Event<MouseClickEventHandler> MouseUp;
		Event<MouseClickEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<UIElementEventHandler> GotFocus;
		Event<UIElementEventHandler> LostFocus;

		void SetTag(const Any& value, ValueSource source = ValueSource::Local);
		const Any& GetTag() const { return _tag; }

		void SetCornersRadius(float value, ValueSource source = ValueSource::Local);
		float GetCornersRadius() const { return _cornersRadius; }

		void SetIsFocusable(bool value, ValueSource source = ValueSource::Local);
		bool IsFocusable() const { return _focusable; }

		bool IsMouseOver() const { return PseudoClasses.Has(OnHover); }	
		bool IsMousePressed() const { return PseudoClasses.Has(OnPressed); }
		bool IsFocused() const { return PseudoClasses.Has(OnFocus); }

		Ref<UIElement> HitTest(FPoint point);

		void Focus();
		void Render(RenderContext context) override;

		static inline StyleableProperty TagProperty { &SetTag, &GetTag };
		static inline StyleableProperty CornersRadiusProperty { &SetCornersRadius, &GetCornersRadius };
		static inline StyleableProperty IsFocusableProperty { &SetIsFocusable, &IsFocusable };
		static inline const PseudoClass OnHover = PseudoClass::Register("hover");
		static inline const PseudoClass OnPressed = PseudoClass::Register("pressed");
		static inline const PseudoClass OnFocus = PseudoClass::Register("focus");
	protected:
		~UIElement() = default;
		void SetParent(IStyleHost* parent) override;
		void OnCursorChanged(Cursor cursor) final;
		void OnBackgroundChanged(const Brush& background) final;
		void OnDataContextChanged(const Ref<ObservableObject>& dataContext) final;
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		virtual void OnKeyDown(KeyEventArgs e);
		virtual void OnKeyUp(KeyEventArgs e);
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseClickEventArgs& e);
		virtual void OnMouseUp(MouseClickEventArgs& e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnMouseEnter(MouseMoveEventArgs e);
		virtual void OnMouseLeave(MouseMoveEventArgs e);
		virtual void OnGotFocus(EventArgs e);
		virtual void OnLostFocus(EventArgs e);
		void AddChild(const Ref<UIElement>& child);
		void RemoveChild(const Ref<UIElement>& child);
		const std::vector<Ref<UIElement>>& GetChildren() const { return _children; }
	private:
		RenderFragment CreateBackgroundFragment(const Brush& background);
		void InvalidateBackground();
	private:
		RenderFragment _backgroundFragment;
		UIElement* _parent {};
		std::vector<Ref<UIElement>> _children;

		Any _tag;
		float _cornersRadius = 0;
		bool _focusable = false;

		ValueSource _tagSource {};	
		ValueSource _cornersRadiusSource {};
		ValueSource _focusableSource {};

		friend class Window;
		friend class InputManager;
		friend class FocusManager;
	};

	class UIElementDataTemplate final : public IDataTemplate
	{
	public:
		Ref<UIElement> Build(const Ref<ObservableObject>& data) override;
		bool Match(const Ref<ObservableObject>& data) const override;
	};
}