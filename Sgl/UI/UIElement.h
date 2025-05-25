#pragma once

#include "../Style/Trigger.h"
#include "../Style/Layout.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Visual/VisualElement.h"

namespace Sgl::UI
{
	class UIElement: 
		public VisualElement,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		Style ClassStyle;

		Trigger OnHover;
		Trigger OnMousePressed;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseEventHandler> MouseDoubleClick;
		Event<MouseWheelEventHandler> MouseWheel;
		
		float Width = 0;
		float Height = 0;
		float MinWidth = 0;
		float MinHeight = 0;
		float MaxWidth = std::numeric_limits<float>::max();
		float MaxHeight = std::numeric_limits<float>::max();
		size_t ZIndex = 1;
		VisualElement* Tooltip = nullptr;
		Visibility Visibility = Visibility::Visible;
		VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
		Thickness Margin;
		Thickness Padding;
		Border Border;
	private:
		using base = VisualElement;
	public:
		UIElement();
		UIElement(const UIElement& other);
		virtual ~UIElement() = default;

		void OnRender(RenderContext rc) const override;
		void ResetStyle() override;
	protected:
		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			OnMousePressed.Activate();
			MouseDown.TryRaise(*this, e);
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			OnMousePressed.Deactivate();
			MouseUp.TryRaise(*this, e);
		}

		void OnMouseMove(const MouseButtonEventArgs& e) override
		{
			MouseMove.TryRaise(*this, e);
		}

		virtual void OnMouseEnter(const MouseButtonEventArgs& e)
		{
			OnHover.Activate();
			MouseEnter.TryRaise(*this, e);
		}

		virtual void OnMouseLeave(const MouseButtonEventArgs& e)
		{
			OnHover.Deactivate();
			MouseLeave.TryRaise(*this, e);
		}

		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override
		{
			MouseDoubleClick.TryRaise(*this, e);
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			MouseWheel.TryRaise(*this, e);
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			KeyDown.TryRaise(*this, e);
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			KeyUp.TryRaise(*this, e);
		}

	private:
		friend class Layout;
	};
}