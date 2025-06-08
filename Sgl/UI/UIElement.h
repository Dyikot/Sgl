#pragma once

#include "../Style/Trigger.h"
#include "../Style/Layout.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Visual/VisualElement.h"
#include "../Data/Object.h"

namespace Sgl::UI
{
	class UIElement: 
		public VisualElement,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		Style Class;

		Trigger OnHover;
		Trigger OnMousePressed;
		
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
	private:
		using base = VisualElement;

		FPoint _position;
	public:
		UIElement();
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;

		void OnRender(RenderContext rc) const override;
		void ResetStyle() override;
	protected:
		void OnKeyUp(const KeyEventArgs& e) override {}
		void OnKeyDown(const KeyEventArgs& e) override {}
		void OnMouseMove(const MouseButtonEventArgs& e) override {}
		void OnMouseWheel(const MouseWheelEventArgs& e) override {}

		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			OnMousePressed.Activate();
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			OnMousePressed.Deactivate();
		}

		virtual void OnMouseEnter(const MouseButtonEventArgs& e)
		{
			OnHover.Activate();
		}

		virtual void OnMouseLeave(const MouseButtonEventArgs& e)
		{
			OnHover.Deactivate();
		}
	private:
		friend class Layout;
	};

	struct UIElementComparer
	{
		bool operator()(const std::shared_ptr<UIElement>& left,
						const std::shared_ptr<UIElement>& right) const
		{
			return left->ZIndex < right->ZIndex;
		}
	};
}