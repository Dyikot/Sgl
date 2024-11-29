#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement(const Style& style) noexcept
	{
		style.TryInit(MouseDownProperty, MouseDown);
		style.TryInit(MouseUpProperty, MouseUp);
		style.TryInit(MouseMoveProperty, MouseMove);
		style.TryInit(MouseEnterProperty, MouseEnter);
		style.TryInit(MouseLeaveProperty, MouseLeave);
		style.TryInit(KeyUpProperty, KeyUp);
		style.TryInit(KeyDownProperty, KeyDown);
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

	void UIElement::OnMouseEnter(const MouseButtonEventArgs& e)
	{
		if(MouseEnter)
		{
			MouseEnter(this, e);
		}
	}

	void UIElement::OnMouseLeave(const MouseButtonEventArgs& e)
	{
		if(MouseLeave)
		{
			MouseLeave(this, e);
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