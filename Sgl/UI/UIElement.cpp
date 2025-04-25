#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement()
	{
		AddProperty<std::reference_wrapper<const Cursor>>(cursorProperty, Cursors::arrow.Get());
	}

	void UIElement::AddStyle(const Style& style)
	{
		
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		MouseDown.TryRaise(*this, e);
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		MouseUp.TryRaise(*this, e);
	}

	void UIElement::OnMouseMove(const MouseButtonEventArgs& e)
	{
		MouseMove.TryRaise(*this, e);
	}

	void UIElement::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		MouseDoubleClick.TryRaise(*this, e);
	}

	void UIElement::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		MouseWheel.TryRaise(*this, e);
	}

	void UIElement::OnKeyDown(const KeyEventArgs& e)
	{
		KeyDown.TryRaise(*this, e);
	}

	void UIElement::OnKeyUp(const KeyEventArgs& e)
	{
		KeyUp.TryRaise(*this, e);
	}
}