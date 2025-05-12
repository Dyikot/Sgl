#pragma once

#include <stack>
#include "../Graphic/IVisual.h"
#include "../Events/Event.h"
#include "../Style/Style.h"
#include "../Events/UIElementEvents.h"

namespace Sgl
{
	class UIElement: public IVisual
	{
	public:
		Style Style;
		StyleClass Class;
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseDoubleClick;
		Event<MouseWheelEventHandler> MouseWheel;
	public:
		UIElement();
		virtual ~UIElement() = default;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e)
		{
			MouseDown.TryRaise(*this, e);
		}

		virtual void OnMouseUp(const MouseButtonEventArgs& e)
		{
			MouseUp.TryRaise(*this, e);
		}

		virtual void OnMouseMove(const MouseButtonEventArgs& e)
		{
			MouseMove.TryRaise(*this, e);
		}

		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e)
		{
			MouseDoubleClick.TryRaise(*this, e);
		}

		virtual void OnMouseWheel(const MouseWheelEventArgs& e)
		{
			MouseWheel.TryRaise(*this, e);
		}

		virtual void OnKeyDown(const KeyEventArgs& e)
		{
			KeyDown.TryRaise(*this, e);
		}

		virtual void OnKeyUp(const KeyEventArgs& e)
		{
			KeyUp.TryRaise(*this, e);
		}
	};
}