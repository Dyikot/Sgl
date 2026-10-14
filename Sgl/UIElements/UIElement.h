#pragma once

#include "../Base/Any.h"
#include "../Data/IDataTemplate.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Layout/Layoutable.h"
#include "../Styling/StyleContext.h"

namespace Sgl
{
	class Window;

	class UIElement : public Layoutable
	{
	private:
		using UIElementEventHandler = EventHandler<UIElement>;
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs&>;
		using MouseMoveEventHandler = EventHandler<UIElement, MouseMoveEventArgs>;
		using MouseClickEventHandler = EventHandler<UIElement, MouseClickEventArgs&>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs&>;
	public:
		UIElement() = default;

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

		void SetTag(const Any& value);
		const Any& GetTag() const { return _tag; }

		void SetCornersRadius(float value);
		float GetCornersRadius() const { return _cornersRadius; }

		void SetIsFocusable(bool value);
		bool IsFocusable() const { return _focusable; }

		void SetClipToBounds(bool value);
		bool GetClipToBounds() const { return _clipToBounds; }

		bool IsMouseOver() const { return States.Has(OnHover); }	
		bool IsMousePressed() const { return States.Has(OnPressed); }
		bool IsFocused() const { return States.Has(OnFocus); }

		Ref<UIElement> HitTest(FPoint point);

		bool Focus();
		void Render(RenderContext& context) final;
		void ApplyStyling();
		Ref<UIElement> FindChild(const std::string& name);

		static inline const VisualState OnHover = VisualState::Register("hover");
		static inline const VisualState OnPressed = VisualState::Register("pressed");
		static inline const VisualState OnFocus = VisualState::Register("focus");

		static inline StyleableProperty TagProperty { &SetTag, &GetTag };
		static inline StyleableProperty CornersRadiusProperty { &SetCornersRadius, &GetCornersRadius };
		static inline StyleableProperty IsFocusableProperty { &SetIsFocusable, &IsFocusable };
		static inline StyleableProperty ClipToBoundsProperty { &SetClipToBounds, &GetClipToBounds };
	protected:
		~UIElement() = default;
		void OnPropertyChanged(PropertyBase& property) override;
		void SetParent(Styleable* parent) override;
		void OnCursorChanged(Cursor cursor) final;
		void OnBackgroundChanged(const Brush& background) final;
		void OnDataContextChanged(const Ref<ObservableObject>& dataContext) final;
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
		virtual void OnRender(RenderContext& context) {}
		virtual void OnKeyDown(KeyEventArgs& e);
		virtual void OnKeyUp(KeyEventArgs& e);
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
		void ClearChildren();
		UIElement* GetParent() const { return _parent; }
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
		bool _clipToBounds = false;
		bool _renderBackground = false;

		ValueSource _tagSource {};	
		ValueSource _cornersRadiusSource {};
		ValueSource _focusableSource {};
		ValueSource _clipToBoundsSource {};

		friend class Window;
		friend class InputManager;
		friend class FocusManager;
	};

	class UIElementStyle : public Style
	{
	public:
		void OnBase(StyleContext& context) override {}
		void OnState(StyleContext& context, const VisualStateSet& states) override;
	protected:
		virtual void OnHover(StyleContext& context) {}
		virtual void OnPressed(StyleContext& context) {}
		virtual void OnFocus(StyleContext& context) {}
	};

	class UIElementDataTemplate final : public IDataTemplate
	{
	public:
		Ref<UIElement> Build(const Ref<ObservableObject>& data) override;
		bool Match(const Ref<ObservableObject>& data) const override;
	};
}