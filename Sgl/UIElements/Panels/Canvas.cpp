#include "Canvas.h"

namespace Sgl::UIElements
{
	struct Context
	{
		int Left = 0;
		int Top = 0;
		int Right = 0;
		int Bottom = 0;
	};

	LayoutProperty<int> Canvas::LeftProperty =
	{
		[](Layoutable& element, int value)
		{
			if(element.SetProperty(LeftProperty, element.GetLayoutContext<Context>().Left, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Left; }
	};

	LayoutProperty<int> Canvas::TopProperty =
	{
		[](Layoutable& element, int value)
		{
			if(element.SetProperty(TopProperty, element.GetLayoutContext<Context>().Top, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Top; }
	};

	LayoutProperty<int> Canvas::RightProperty =
	{
		[](Layoutable& element, int value)
		{
			if(element.SetProperty(RightProperty, element.GetLayoutContext<Context>().Left, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Right; }
	};

	LayoutProperty<int> Canvas::BottomProperty =
	{
		[](Layoutable& element, int value)
		{
			if(element.SetProperty(BottomProperty, element.GetLayoutContext<Context>().Bottom, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Bottom; }
	};

	void Canvas::SetLeft(const Ref<UIElement>& element, int value)
	{
		if(element->SetProperty(LeftProperty, element->GetLayoutContext<Context>().Left, value))
		{
			element->InvalidateMeasure();
		}
	}

	int Canvas::GetLeft(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Left;
	}

	void Canvas::SetTop(const Ref<UIElement>&element, int value)
	{
		if(element->SetProperty(TopProperty, element->GetLayoutContext<Context>().Top, value))
		{
			element->InvalidateMeasure();
		}
	}

	int Canvas::GetTop(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Top;
	}

	void Canvas::SetRight(const Ref<UIElement>&element, int value)
	{
		if(element->SetProperty(RightProperty, element->GetLayoutContext<Context>().Right, value))
		{
			element->InvalidateMeasure();
		}
	}

	int Canvas::GetRight(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Right;
	}

	void Canvas::SetBottom(const Ref<UIElement>&element, int value)
	{
		if(element->SetProperty(BottomProperty, element->GetLayoutContext<Context>().Bottom, value))
		{
			element->InvalidateMeasure();
		}
	}

	int Canvas::GetBottom(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Bottom;
	}

	FSize Canvas::MeasureContent(FSize availableSize)
	{
		for(auto& child : Children)
		{
			child->Measure(availableSize);
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