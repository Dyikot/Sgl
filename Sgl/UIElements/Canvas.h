#pragma once

#include "../UIElement/Panel.h"

namespace Sgl::UIElements
{
	class Canvas : public Panel
	{
	public:
		Canvas() = default;
		Canvas(const Canvas&) = default;
		Canvas(Canvas&&) = default;

		static inline AttachedProperty<int> LeftProperty;
		static inline AttachedProperty<int> TopProperty;
		static inline AttachedProperty<int> RightProperty;
		static inline AttachedProperty<int> BottomProperty;
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
		
		int GetLeft(UIElement& element)
		{
			return element.GetAttachedValue(LeftProperty);
		}

		int GetTop(UIElement& element)
		{
			return element.GetAttachedValue(TopProperty);
		}

		int GetRight(UIElement& element)
		{
			return element.GetAttachedValue(RightProperty);
		}

		int GetBottom(UIElement& element)
		{
			return element.GetAttachedValue(BottomProperty);
		}
	};
}