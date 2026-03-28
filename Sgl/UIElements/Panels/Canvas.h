#pragma once

#include "Panel.h"
#include "../../Data/LayoutProperty.h"

namespace Sgl::UIElements
{
	class Canvas : public Panel
	{
	public:
		struct Context
		{
			int Left = 0;
			int Top = 0;
			int Right = 0;
			int Bottom = 0;
		};
	public:
		Canvas() = default;
		Canvas(const Canvas&) = default;
		Canvas(Canvas&&) = default;

		static void SetLeft(const Ref<UIElement>& element, int value);
		static int GetLeft(const Ref<UIElement>& element) { return element->GetLayoutContext<Context>().Left; }

		static void SetTop(const Ref<UIElement>& element, int value);
		static int GetTop(const Ref<UIElement>& element) { return element->GetLayoutContext<Context>().Top; }

		static void SetRight(const Ref<UIElement>& element, int value);
		static int GetRight(const Ref<UIElement>& element) { return element->GetLayoutContext<Context>().Right; }

		static void SetBottom(const Ref<UIElement>& element, int value);
		static int GetBottom(const Ref<UIElement>& element) { return element->GetLayoutContext<Context>().Bottom; }

		static LayoutProperty<int> LeftProperty;
		static LayoutProperty<int> TopProperty;
		static LayoutProperty<int> RightProperty;
		static LayoutProperty<int> BottomProperty;
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;	
	};
}