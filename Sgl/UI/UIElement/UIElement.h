#pragma once

#include <set>
#include "UIElementsCollection.h"
#include "../../Events/Event.h"
#include "../../Style/Trigger.h"
#include "../../Events/MouseAndKeyEvents.h"
#include "../../Graphic/Primitives.h"
#include "../VisualElement.h"
#include "Properties.h"

namespace Sgl
{
	class UIElement: 
		public VisualElement,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	private:
		using base = VisualElement;
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

		UIElementsCollection Children;

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
		bool _isHover = false;
	public:
		UIElement();
		virtual ~UIElement() = default;

		void OnRender(RenderContext renderContext) const override;
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

		void OnMouseEnter(const MouseButtonEventArgs& e)
		{
			_isHover = true;
			OnHover.Activate();
			MouseEnter.TryRaise(*this, e);
		}

		void OnMouseLeave(const MouseButtonEventArgs& e)
		{
			_isHover = false;
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

		friend class UIElementsCollection;
	};
}