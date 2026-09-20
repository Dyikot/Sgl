#include "Canvas.h"

namespace
{
	struct Context
	{
		int Left;
		int Top;
		int Right;
		int Bottom;
	};

	bool SetField(int& field, int value)
	{
		if(field == value)
		{
			return false;
		}

		field = value;
		return true;
	}
}

namespace Sgl::UIElements
{
	Canvas::Canvas()
	{
		Name = "Canvas";
	}

	void Canvas::SetLeft(const Ref<UIElement>& element, int value)
	{
		if(SetField(element->GetLayoutContext<Context>().Left, value))
		{
			element->InvalidateArrange();
		}
	}

	int Canvas::GetLeft(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Left;
	}

	void Canvas::SetTop(const Ref<UIElement>& element, int value)
	{
		if(SetField(element->GetLayoutContext<Context>().Top, value))
		{
			element->InvalidateArrange();
		}
	}

	int Canvas::GetTop(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Top;
	}

	void Canvas::SetRight(const Ref<UIElement>& element, int value)
	{
		if(SetField(element->GetLayoutContext<Context>().Right, value))
		{
			element->InvalidateArrange();
		}
	}

	int Canvas::GetRight(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Right;
	}

	void Canvas::SetBottom(const Ref<UIElement>& element, int value)
	{
		if(SetField(element->GetLayoutContext<Context>().Bottom, value))
		{
			element->InvalidateArrange();
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