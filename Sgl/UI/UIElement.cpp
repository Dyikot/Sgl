#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement(const Style& style) noexcept
	{
		style.TryAddHandlerToEvent(MouseDownProperty, MouseDown);
		style.TryAddHandlerToEvent(MouseUpProperty, MouseUp);
		style.TryAddHandlerToEvent(MouseMoveProperty, MouseMove);
		style.TryAddHandlerToEvent(KeyUpProperty, KeyUp);
		style.TryAddHandlerToEvent(KeyDownProperty, KeyDown);
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