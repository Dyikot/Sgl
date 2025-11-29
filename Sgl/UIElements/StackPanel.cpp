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

		if(_orientation == Orientation::Vertical)
		{
			for(auto& child : Children)
			{
				child->Measure(avaliableSize);
				auto [width, height] = child->GetDesiredSize();

				size.Width = std::max(size.Width, width);
				size.Height += height;
			}

			size.Height += _spacing * (Children.Count() - 1);
		}
		else
		{
			for(auto& child : Children)
			{
				child->Measure(avaliableSize);
				auto [width, height] = child->GetDesiredSize();

				size.Width += width;
				size.Height = std::max(size.Height, height);
			}

			size.Width += _spacing * (Children.Count() - 1);
		}

		return size;
	}

	void StackPanel::ArrangeContent(FRect rect)
	{
		if(_orientation == Orientation::Vertical)
		{
			float totalHeight = 0;

			for(auto& child : Children)
			{
				auto [_, height] = child->GetDesiredSize();

				FRect childRect = rect;
				childRect.y += totalHeight;
				childRect.h = height;

				child->Arrange(childRect);

				totalHeight += height + _spacing;
			}
		}
		else
		{
			float totalWidth = 0;

			for(auto& child : Children)
			{
				auto [width, _] = child->GetDesiredSize();

				FRect childRect = rect;
				childRect.x += totalWidth;
				childRect.w = width;

				child->Arrange(childRect);

				totalWidth += width + _spacing;
			}
		}		
	}
}
