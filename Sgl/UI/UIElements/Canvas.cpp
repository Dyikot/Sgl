#include "Canvas.h"

namespace Sgl::UIElements
{
	void Canvas::SetLeft(UIElement& element, int value)
	{
		element.SetAttachProperty(LeftProperty, value);
	}

	int Canvas::GetLeft(UIElement& element)
	{
		return element.GetAttachProperty(LeftProperty);
	}

	void Canvas::SetTop(UIElement& element, int value)
	{
		element.SetAttachProperty(TopProperty, value);
	}

	int Canvas::GetTop(UIElement& element)
	{
		return element.GetAttachProperty(TopProperty);
	}

	void Canvas::SetRight(UIElement& element, int value)
	{
		element.SetAttachProperty(RightProperty, value);
	}

	int Canvas::GetRight(UIElement& element)
	{
		return element.GetAttachProperty(RightProperty);
	}

	void Canvas::SetBottom(UIElement& element, int value)
	{
		element.SetAttachProperty(BottomProperty, value);
	}

	int Canvas::GetBottom(UIElement& element)
	{
		return element.GetAttachProperty(BottomProperty);
	}

	void Canvas::Render(RenderContext context)
	{
		RenderBackground(context);
		Panel::Render(context);
	}

	void Canvas::ArrangeContent(FRect rect)
	{
		for(auto& child : Children)
		{
			FRect childRect = rect;

			auto left = GetLeft(child.GetValue());
			auto top = GetTop(child.GetValue());
			auto right = GetRight(child.GetValue());
			auto bottom = GetBottom(child.GetValue());

			bool isRightValid = right != 0;
			bool isBottomValid = bottom != 0;

			FSize avaliableSize =
			{
				.Width = isRightValid ? rect.w - right : rect.w + left,
				.Height = isBottomValid ? rect.h - bottom : rect.h + top
			};

			child->Measure(avaliableSize);
			auto [width, height] = child->GetDesiredSize();

			if(isRightValid)
			{
				childRect.x += rect.w - right - width;
				childRect.w -= right;
			}
			else
			{
				childRect.x += left;
				childRect.w -= left;
			}

			if(isBottomValid)
			{
				childRect.y += rect.h - bottom - height;
				childRect.h -= bottom;
			}
			else
			{
				childRect.y += top;
				childRect.h -= top;
			}
			
			child->Arrange(childRect);
		}
	}
}