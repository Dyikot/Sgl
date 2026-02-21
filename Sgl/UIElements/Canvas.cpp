#include "Canvas.h"

namespace Sgl::UIElements
{
	FSize Canvas::MeasureContent(FSize avaliableSize)
	{
		for(auto& child : Children)
		{
			child->Measure(avaliableSize);
		}

		return FSize();
	}

	void Canvas::ArrangeContent(FRect rect)
	{
		for(auto& child : Children)
		{
			int left = GetLeft(child);
			int top = GetTop(child);

			auto [width, height] = child->GetDesiredSize();

			FRect childRect =
			{
				.w = width,
				.h = height
			};

			childRect.x = rect.x + (left != 0 ? left : rect.w - GetRight(child) - width);
			childRect.y = rect.y + (top != 0 ? top : rect.h - GetBottom(child) - height);
			
			child->Arrange(childRect);
		}
	}	
}