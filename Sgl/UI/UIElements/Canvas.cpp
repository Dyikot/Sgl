#include "Canvas.h"

namespace Sgl::UIElements
{
	/*void Canvas::Arrange()
	{
		if(!_shouldArrange)
		{
			return;
		}

		for(auto& child : Children)
		{
			auto [left, top, right, bottom] = child->Margin.Get();
			auto position = GetActualPosition();
			position.x += left >= right ? left : GetActualWidth() - right - child->GetActualWidth();
			position.y += top >= bottom ? top : GetActualHeight() - bottom - child->GetActualHeight();
			SetPosition(*child, position);

			_shouldArrange = false;
		}
	}*/
}