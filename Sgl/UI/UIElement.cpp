#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement(const Style& style) noexcept
	{
		style.EventSetters.TrySetEvent(MouseDownEvent, MouseDown);
		style.EventSetters.TrySetEvent(MouseUpEvent, MouseUp);
		style.EventSetters.TrySetEvent(MouseMoveEvent, MouseMove);
		style.EventSetters.TrySetEvent(MouseWheelEvent, MouseWheel);
		style.EventSetters.TrySetEvent(KeyDownEvent, KeyDown);
		style.EventSetters.TrySetEvent(KeyUpEvent, KeyUp);
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		if(MouseDown)
		{
			MouseDown(this, e);
		}
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		if(MouseUp)
		{
			MouseUp(this, e);
		}
	}

	void UIElement::OnMouseMove(const MouseButtonEventArgs& e)
	{
		if(MouseMove)
		{
			MouseMove(this, e);
		}
	}

	void UIElement::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		if(MouseWheel)
		{
			MouseWheel(this, e);
		}
	}

	void UIElement::OnKeyDown(const KeyEventArgs& e)
	{
		if(KeyDown)
		{
			KeyDown(this, e);
		}
	}

	void UIElement::OnKeyUp(const KeyEventArgs& e)
	{
		if(KeyUp)
		{
			KeyUp(this, e);
		}
	}
}