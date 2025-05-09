#include "UIElement.h"

namespace Sgl
{
	void UIElement::SetClassStyle()
	{
		style = {};
		_classStyleSetter(style);
	}

	void UIElement::SetStyle(const StyleSetter& setter)
	{
		SetClassStyle();
		setter(style);
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		onMouseDown.TryRaise(*this, e);
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		onMouseUp.TryRaise(*this, e);
	}

	void UIElement::OnMouseMove(const MouseButtonEventArgs& e)
	{
		onMouseMove.TryRaise(*this, e);
	}

	void UIElement::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		onMouseDoubleClick.TryRaise(*this, e);
	}

	void UIElement::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		onMouseWheel.TryRaise(*this, e);
	}

	void UIElement::OnKeyDown(const KeyEventArgs& e)
	{
		onKeyDown.TryRaise(*this, e);
	}

	void UIElement::OnKeyUp(const KeyEventArgs& e)
	{
		onKeyUp.TryRaise(*this, e);
	}
}