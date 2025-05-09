#pragma once

#include "../Graphic/IVisual.h"
#include "../Events/Event.h"
#include "../Style/Style.h"
#include <stack>

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
		using StyleSetter = Action<Style&>;

		Style style;
		Event<KeyEventHandler> onKeyUp;
		Event<KeyEventHandler> onKeyDown;
		Event<MouseEventHandler> onMouseDown;
		Event<MouseEventHandler> onMouseUp;
		Event<MouseEventHandler> onMouseMove;
		Event<MouseEventHandler> onMouseDoubleClick;
		Event<MouseWheelEventHandler> onMouseWheel;
	private:
		StyleSetter _classStyleSetter;
	public:
		virtual ~UIElement() = default;

		template<StyleSelector... selectors>
		void AddClassStyle()
		{
			_classStyleSetter = [](Style& style) { (selectors(style), ...); };
			_classStyleSetter(style);
		}
	protected:
		void SetClassStyle();
		void SetStyle(const StyleSetter& setter);

		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
	};
}