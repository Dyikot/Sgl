#include "Canvas.h"

namespace Sgl::UIElements
{
	void Canvas::SetLeft(UIElement& element, int value)
	{
		element.SetAttachProperty(LeftProperty, value);
	}

	int Canvas::GetLeft(const UIElement& element)
	{
		return element.GetAttachProperty(LeftProperty);
	}

	void Canvas::SetTop(UIElement& element, int value)
	{
		element.SetAttachProperty(TopProperty, value);
	}

	int Canvas::GetTop(const UIElement& element)
	{
		return element.GetAttachProperty(TopProperty);
	}

	void Canvas::SetRight(UIElement& element, int value)
	{
		element.SetAttachProperty(RightProperty, value);
	}

	int Canvas::GetRight(const UIElement& element)
	{
		return element.GetAttachProperty(RightProperty);
	}

	void Canvas::SetBottom(UIElement& element, int value)
	{
		element.SetAttachProperty(BottomProperty, value);
	}

	int Canvas::GetBottom(const UIElement& element)
	{
		return element.GetAttachProperty(BottomProperty);
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
			int left = GetLeft(*child);
			int top = GetTop(*child);

			auto [width, height] = child->GetDesiredSize();

			FRect childRect =
			{
				.w = width,
				.h = height
			};

			childRect.x = rect.x + (left != 0 ? left : rect.w - GetRight(*child) - width);
			childRect.y = rect.y + (top != 0 ? top : rect.h - GetBottom(*child) - height);
			
			child->Arrange(childRect);
		}
	}	
}