#pragma once

#include "Panel.h"
#include "../../Data/LayoutProperty.h"

namespace Sgl::UIElements
{
	class Canvas : public Panel
	{
	public:
		Canvas() = default;
		Canvas(const Canvas&) = delete;
		Canvas(Canvas&&) = default;

		static void SetLeft(const Ref<UIElement>& element, int value);
		static int GetLeft(const Ref<UIElement>& element);

		static void SetTop(const Ref<UIElement>& element, int value);
		static int GetTop(const Ref<UIElement>& element);

		static void SetRight(const Ref<UIElement>& element, int value);
		static int GetRight(const Ref<UIElement>& element);

		static void SetBottom(const Ref<UIElement>& element, int value);
		static int GetBottom(const Ref<UIElement>& element);

		static LayoutProperty<int> LeftProperty;
		static LayoutProperty<int> TopProperty;
		static LayoutProperty<int> RightProperty;
		static LayoutProperty<int> BottomProperty;
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;	
	};
}