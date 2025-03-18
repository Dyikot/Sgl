#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement()
	{
		AddProperty<std::reference_wrapper<const Cursor>>(CursorProperty, Cursors::Arrow.Value());
	}

	void UIElement::AddStyle(const Style& style)
	{
		
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		MouseDown.TryInvoke(this, e);
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		MouseUp.TryInvoke(this, e);
	}

	void UIElement::OnMouseMove(const MouseButtonEventArgs& e)
	{
		MouseMove.TryInvoke(this, e);
	}

	void UIElement::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		MouseDoubleClick.TryInvoke(this, e);
	}

	void UIElement::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		MouseWheel.TryInvoke(this, e);
	}

	void UIElement::OnKeyDown(const KeyEventArgs& e)
	{
		KeyDown.TryInvoke(this, e);
	}

	void UIElement::OnKeyUp(const KeyEventArgs& e)
	{
		KeyUp.TryInvoke(this, e);
	}
}