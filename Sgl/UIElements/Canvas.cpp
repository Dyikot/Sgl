#include "Canvas.h"

namespace Sgl::UIElements
{
	LayoutProperty<int> Canvas::LeftProperty =
	{
		[](Layoutable& element, int value)
		{
			element.SetProperty(LeftProperty, element.GetLayoutContext<Context>().Left, value);
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Left; }
	};

	LayoutProperty<int> Canvas::TopProperty =
	{
		[](Layoutable& element, int value)
		{
			element.SetProperty(TopProperty, element.GetLayoutContext<Context>().Top, value);
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Top; }
	};

	LayoutProperty<int> Canvas::RightProperty =
	{
		[](Layoutable& element, int value)
		{
			element.SetProperty(RightProperty, element.GetLayoutContext<Context>().Left, value);
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Right; }
	};

	LayoutProperty<int> Canvas::BottomProperty =
	{
		[](Layoutable& element, int value)
		{
			element.SetProperty(BottomProperty, element.GetLayoutContext<Context>().Bottom, value);
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Bottom; }
	};

	void Canvas::SetLeft(const Ref<UIElement>& element, int value)
	{
		element->SetProperty(LeftProperty, element->GetLayoutContext<Context>().Left, value);
	}

	void Canvas::SetTop(const Ref<UIElement>&element, int value)
	{
		element->SetProperty(TopProperty, element->GetLayoutContext<Context>().Top, value);
	}

	void Canvas::SetRight(const Ref<UIElement>&element, int value)
	{
		element->SetProperty(RightProperty, element->GetLayoutContext<Context>().Right, value);
	}

	void Canvas::SetBottom(const Ref<UIElement>&element, int value)
	{
		element->SetProperty(BottomProperty, element->GetLayoutContext<Context>().Bottom, value);
	}

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