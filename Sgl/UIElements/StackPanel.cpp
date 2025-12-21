#include "StackPanel.h"

namespace Sgl
{
	StackPanel::StackPanel(const StackPanel& other) :
		Panel(other),
		_spacing(other._spacing),
		_orientation(other._orientation)
	{}

	StackPanel::StackPanel(StackPanel && other) noexcept :
		Panel(std::move(other)),
		_spacing(other._spacing),
		_orientation(other._orientation)
	{}

	void StackPanel::SetSpacing(float value)
	{
		if(SetProperty(SpacingProperty, _spacing, value))
		{
			InvalidateMeasure();
		}
	}

	void StackPanel::SetOrientation(Orientation value)
	{
		if(SetProperty(OrientationProperty, _orientation, value))
		{
			InvalidateMeasure();
		}
	}

	FSize StackPanel::MeasureContent(FSize avaliableSize)
	{
		FSize size = {};

		if(Children.Count() == 0)
		{
			return size;
		}

		size_t visibleChildren = Children.Count();

		if(_orientation == Orientation::Vertical)
		{
			for(auto& child : Children)
			{
				if(!child->IsVisible())
				{
					visibleChildren--;
				}

				child->Measure(avaliableSize);
				auto [width, height] = child->GetDesiredSize();

				size.Width = std::max(size.Width, width);
				size.Height += height;
			}

			if(visibleChildren > 1)
			{
				size.Height += _spacing * (visibleChildren - 1);	
			}
		}
		else
		{
			for(auto& child : Children)
			{
				if(!child->IsVisible())
				{
					visibleChildren--;
				}

				child->Measure(avaliableSize);
				auto [width, height] = child->GetDesiredSize();

				size.Width += width;
				size.Height = std::max(size.Height, height);
			}

			if(visibleChildren > 1)
			{
				size.Width += _spacing * (visibleChildren - 1);
			}
		}

		return size;
	}

	void StackPanel::ArrangeContent(FRect rect)
	{
		size_t visibleChildren = Children.Count();
		float spacing = _spacing;

		if(_orientation == Orientation::Vertical)
		{
			float totalHeight = 0;

			for(auto& child : Children)
			{
				auto [_, height] = child->GetDesiredSize();
				spacing = child->IsVisible() ? _spacing : 0.f;

				FRect childRect = rect;
				childRect.y += totalHeight;
				childRect.h = height;

				child->Arrange(childRect);

				totalHeight += height + spacing;				
			}
		}
		else
		{
			float totalWidth = 0;

			for(auto& child : Children)
			{
				auto [width, _] = child->GetDesiredSize();
				spacing = child->IsVisible() ? _spacing : 0.f;

				FRect childRect = rect;
				childRect.x += totalWidth;
				childRect.w = width;

				child->Arrange(childRect);

				totalWidth += width + spacing;
			}
		}		
	}
}
