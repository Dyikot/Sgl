#include "Canvas.h"

namespace Sgl::UI
{
	Canvas::Canvas(VisualElement& parent):
		Layout(parent)
	{}

	void Canvas::Arrange()
	{
		if(!_shouldArrange)
		{
			return;
		}

		for(auto& child : Children)
		{
			auto [left, top, right, bottom] = child->GetMargin();
			auto position = GetActualPosition();
			position.x += left >= right ? left : GetActualWidth() - right - child->GetActualWidth();
			position.y += top >= bottom ? top : GetActualHeight() - bottom - child->GetActualHeight();
			SetElementPosition(*child, position);

			_shouldArrange = false;
		}
	}
}