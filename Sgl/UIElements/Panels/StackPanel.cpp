#include "StackPanel.h"

namespace Sgl::UIElements
{
	StackPanel::StackPanel()
	{
		Name = "StackPanel";
	}

	void StackPanel::SetSpacing(float value, ValueSource source)
	{
		SetProperty(SpacingProperty, _spacing, value, _spacingSource, source);
	}

	void StackPanel::SetOrientation(Orientation value, ValueSource source)
	{
		SetProperty(OrientationProperty, _orientation, value, _orientationSource, source);
	}

	void StackPanel::OnPropertyChanged(PropertyBase& property)
	{
		Panel::OnPropertyChanged(property);

		if(property == SpacingProperty || property == OrientationProperty)
		{
			InvalidateMeasure();
		}
	}

	FSize StackPanel::MeasureContent(FSize availableSize)
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

				child->Measure(availableSize);
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

				child->Measure(availableSize);
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
