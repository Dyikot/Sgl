#include "Canvas.h"

namespace Sgl::UI
{
	void Canvas::Arrange()
	{
		if(!_shouldArrange)
		{
			return;
		}

		for(auto& child : GetChildren())
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