#pragma once

#include "../Base/Any.h"
#include "../Data/IDataTemplate.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Layout/Layoutable.h"
#include "../Render/RenderFragment.h"

namespace Sgl
{
	class Window;
	class InputManager;

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

		void SetCornersRadius(float value, ValueSource source = ValueSource::Local);
		float GetCornersRadius() const { return _cornersRadius; }

		bool IsMouseOver() const { return PseudoClasses.Has(OnHover); }	
		bool IsMousePressed() const { return PseudoClasses.Has(OnPressed); }

		static Ref<UIElement> HitTest(const Ref<UIElement>& self, FPoint point);
		void Render(RenderContext context) override;

		static inline StyleableProperty TagProperty { &SetTag, &GetTag };
		static inline StyleableProperty CornersRadiusProperty { &SetCornersRadius, &GetCornersRadius };
	protected:
		void SetParent(IStyleHost* parent) override;
		void OnCursorChanged(Cursor cursor) override;
		void OnBackgroundChanged(const Brush& background) override;
		void OnAttachedToLogicalTree() override;
		virtual std::span<const Ref<UIElement>> GetChildren() const;
		virtual void OnKeyUp(KeyEventArgs e);
		virtual void OnKeyDown(KeyEventArgs e);
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseButtonEventArgs e);
		virtual void OnMouseUp(MouseButtonEventArgs e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnMouseEnter(MouseMoveEventArgs e);
		virtual void OnMouseLeave(MouseMoveEventArgs e);
	private:
		RenderFragment CreateBackgroundFragment(const Brush& background);
		void InvalidateBackground();
	private:
		RenderFragment _backgroundFragment;
		UIElement* _parent {};

		Any _tag;
		float _cornersRadius = 0;

		ValueSource _tagSource {};	
		ValueSource _cornersRadiusSource {};

		friend class Panel;
		friend class Window;
		friend class InputManager;
		friend class ContentUIElement;
	};

	class UIElementDataTemplate : public IDataTemplate
	{
	public:
		Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) override;
		bool Match(const Ref<INotifyPropertyChanged>& data) const override;
	};
}