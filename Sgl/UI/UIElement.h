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

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
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

	template<std::derived_from<UIElement> TUIElement>
	class ElementConfigurer
	{
	private:
		TUIElement& _element;
	public:
		ElementConfigurer(TUIElement& element):
			_element(element)
		{}

		template<Style::StyleClass... Classes>
		ElementConfigurer& Class()
		{
			_element.Class.Use<Classes...>();
			return *this;
		}

		template<Style::StyleClass... Classes>
		ElementConfigurer& OnHover()
		{
			_element.OnHover.Class.Use<Classes...>();
			return *this;
		}

		template<Style::StyleClass... Classes>
		ElementConfigurer& OnMousePressed()
		{
			_element.OnMousePressed.Class.Use<Classes...>();
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& KeyUp(TCallable callable)
		{
			_element.KeyUp += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& KeyDown(TCallable callable)
		{
			_element.KeyDown += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& MouseDown(TCallable callable)
		{
			_element.MouseDown += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& MouseUp(TCallable callable)
		{
			_element.MouseUp += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& MouseMove(TCallable callable)
		{
			_element.MouseMove += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& MouseEnter(TCallable callable)
		{
			_element.MouseEnter += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& MouseLeave(TCallable callable)
		{
			_element.MouseLeave += std::move(callable);
			return *this;
		}

		template<typename TCallable>
		ElementConfigurer& MouseWheel(TCallable callable)
		{
			_element.MouseWheel += std::move(callable);
			return *this;
		}
	};
}