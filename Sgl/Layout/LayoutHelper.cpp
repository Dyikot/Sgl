#include "LayoutHelper.h"
#include "Layoutable.h"

namespace Sgl
{
    FSize MeasureChild(Layoutable* child, FSize availableSize, Thickness padding)
    {
		if(child)
		{
			child->Measure(Shrink(availableSize, padding));
			return Expand(child->GetDesiredSize(), padding);
		}

		return FSize(padding.Left + padding.Right, padding.Bottom + padding.Top);
    }

	void ArrangeChild(Layoutable* child, FRect rect, Thickness padding)
	{
		if(child)
		{
			FRect finalRect = Shrink(rect, padding);

			if(finalRect.w < 0)
			{
				finalRect.w = 0;
			}

			if(finalRect.h < 0)
			{
				finalRect.h = 0;
			}

			child->Arrange(finalRect);
		}
	}
}