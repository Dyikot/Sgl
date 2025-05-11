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
		Style Style;
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseDoubleClick;
		Event<MouseWheelEventHandler> MouseWheel;
	protected:
		static inline auto EmptySelector = [](Sgl::Style& style) {};
	private:
		StyleSelector _styleSelector = EmptySelector;
	public:
		virtual ~UIElement() = default;

		template<StyleSelector... Selectors>
		void AddStyle()
		{
			_styleSelector = CombineSelectors<Selectors...>;
			_styleSelector(Style);
		}
	protected:
		void ApplyStyle(StyleSelector styleSelector);
		void ApplyDefaultStyle();

		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);

		template<StyleSelector... Selectors>
		static void CombineSelectors(Sgl::Style& style)
		{
			(Selectors(style), ...);
		}
	};
}