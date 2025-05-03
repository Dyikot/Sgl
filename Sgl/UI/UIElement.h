#pragma once

#include "../Graphic/IVisual.h"
#include "../Events/Event.h"
#include "../Style/Style.h"

namespace Sgl
{
	class UIElement;

	using UIElementEventHandler = EventHandler<UIElement, EventArgs>;
	using MouseEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
	using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;

	class UIElement: public IVisual
	{
	public:
		std::unique_ptr<Style> style = std::make_unique<Style>();

		Event<KeyEventHandler> onKeyUp;
		Event<KeyEventHandler> onKeyDown;
		Event<MouseEventHandler> onMouseDown;
		Event<MouseEventHandler> onMouseUp;
		Event<MouseEventHandler> onMouseMove;
		Event<MouseEventHandler> onMouseDoubleClick;
		Event<MouseWheelEventHandler> onMouseWheel;
	public:
		UIElement() = default;

		template<CAction<Style&>... TStyleSelector>
		UIElement(TStyleSelector&&... styleSelectors)
		{
			(std::forward<TStyleSelector>(styleSelectors)(*style), ...);
		}

		virtual ~UIElement() = default;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
	};
}