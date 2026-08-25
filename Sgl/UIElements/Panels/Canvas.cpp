#include "Canvas.h"

namespace Sgl::UIElements
{
	struct Context
	{
		int Left;
		int Top;
		int Right;
		int Bottom;
	};

	LayoutProperty<int> Canvas::LeftProperty =
	{
		[](Layoutable& element, int value)
		{
			if(element.SetProperty(LeftProperty, element.GetLayoutContext<Context>().Left, value))
			{
				element.InvalidateArrange();
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
				element.InvalidateArrange();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Top; }
	};

	LayoutProperty<int> Canvas::RightProperty =
	{
		[](Layoutable& element, int value)
		{
			if(element.SetProperty(RightProperty, element.GetLayoutContext<Context>().Right, value))
			{
				element.InvalidateArrange();
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
				element.InvalidateArrange();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Bottom; }
	};

	Canvas::Canvas()
	{
		Name = "Canvas";
	}

	void Canvas::SetLeft(const Ref<UIElement>& element, int value)
	{
		LeftProperty.InvokeSetter(element.GetValue(), value);
	}

	int Canvas::GetLeft(const Ref<UIElement>& element)
	{
		return LeftProperty.InvokeGetter(element.GetValue());
	}

	void Canvas::SetTop(const Ref<UIElement>& element, int value)
	{
		TopProperty.InvokeSetter(element.GetValue(), value);
	}

	int Canvas::GetTop(const Ref<UIElement>& element)
	{
		return TopProperty.InvokeGetter(element.GetValue());
	}

	void Canvas::SetRight(const Ref<UIElement>& element, int value)
	{
		RightProperty.InvokeSetter(element.GetValue(), value);
	}

	int Canvas::GetRight(const Ref<UIElement>& element)
	{
		return RightProperty.InvokeGetter(element.GetValue());
	}

	void Canvas::SetBottom(const Ref<UIElement>& element, int value)
	{
		BottomProperty.InvokeSetter(element.GetValue(), value);
	}

	int Canvas::GetBottom(const Ref<UIElement>& element)
	{
		return BottomProperty.InvokeGetter(element.GetValue());
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