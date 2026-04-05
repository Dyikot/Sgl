#include "WrapPanel.h"

namespace Sgl::UIElements
{
	WrapPanel::WrapPanel(WrapPanel && other) noexcept:
		Panel(std::move(other)),
		_verticalSpacing(other._verticalSpacing),
		_horizontalSpacing(other._horizontalSpacing),
		_orientation(other._orientation)
	{}

	void WrapPanel::SetVerticalSpacing(float value, ValueSource source)
	{
		if(SetProperty(VeritcalSpacingProperty, _verticalSpacing, value, _verticalSpacingSource, source))
		{
			InvalidateMeasure();
		}
	}

	void WrapPanel::SetHorizontalSpacing(float value, ValueSource source)
	{
		if(SetProperty(HorizontalSpacingProperty, _horizontalSpacing, value, _horizontalSpacingSource, source))
		{
			InvalidateMeasure();
		}
	}

	void WrapPanel::SetOrientation(Orientation value, ValueSource source)
	{
		if(SetProperty(OrientationProperty, _orientation, value, _orientationSource, source))
		{
			InvalidateMeasure();
		}
	}

	FSize WrapPanel::MeasureContent(FSize avaliableSize)
	{
		FSize size = {};

		if(Children.Count() == 0)
		{
			return size;
		}

		if(_orientation == Orientation::Vertical)
		{
			float columnWidth = 0;
			float columnHeight = 0;
			float totalWidth = 0;
			size_t visibleInColumn = 0;

			for(auto& child : Children)
			{
				if(!child->IsVisible())
				{
					continue;
				}

				child->Measure(avaliableSize);
				auto [width, height] = child->GetDesiredSize();

				if(columnHeight + height > avaliableSize.Height)
				{
					totalWidth += columnWidth;
					
					if(visibleInColumn > 0)
					{
						totalWidth += _horizontalSpacing;
					}

					size.Height = std::max(size.Height, columnHeight);
					columnWidth = 0;
					columnHeight = 0;
					visibleInColumn = 0;
				}

				if(columnHeight > 0)
				{
					columnHeight += _verticalSpacing;
				}

				columnHeight += height;
				columnWidth = std::max(columnWidth, width);
				visibleInColumn++;
			}

			totalWidth += columnWidth;
			size.Height = std::max(size.Height, columnHeight);
			size.Width = totalWidth;
		}
		else
		{
			float rowWidth = 0;
			float rowHeight = 0;
			float totalHeight = 0;
			size_t visibleInRow = 0;

			for(auto& child : Children)
			{
				if(!child->IsVisible())
				{
					continue;
				}

				child->Measure(avaliableSize);
				auto [width, height] = child->GetDesiredSize();

				if(rowWidth + width > avaliableSize.Width)
				{
					totalHeight += rowHeight;

					if(visibleInRow > 0)
					{
						totalHeight += _verticalSpacing;
					}

					size.Width = std::max(size.Width, rowWidth);
					rowWidth = 0;
					rowHeight = 0;
					visibleInRow = 0;
				}

				if(rowWidth > 0)
				{
					rowWidth += _horizontalSpacing;
				}

				rowWidth += width;
				rowHeight = std::max(rowHeight, height);
				visibleInRow++;
			}

			totalHeight += rowHeight;
			size.Width = std::max(size.Width, rowWidth);
			size.Height = totalHeight;
		}

		return size;
	}

	void WrapPanel::ArrangeContent(FRect rect)
	{
		if(Children.Count() == 0)
		{
			return;
		}

		if(_orientation == Orientation::Vertical)
		{
			FPoint current = { rect.x, rect.y };
			float columnWidth = 0;
			float columnHeight = 0;
			size_t visibleInLine = 0;

			for(auto& child : Children)
			{
				if(!child->IsVisible())
				{
					continue;
				}

				auto [width, height] = child->GetDesiredSize();

				if(columnHeight + height > rect.h)
				{
					current.x += columnWidth;

					if(visibleInLine > 0)
					{
						current.x += _horizontalSpacing;
					}

					current.y = rect.y;
					columnWidth = 0;
					columnHeight = 0;
					visibleInLine = 0;
				}

				if(columnHeight > 0)
				{
					current.y += _verticalSpacing;
					columnHeight += _verticalSpacing;
				}

				columnHeight += height;
				columnWidth = std::max(columnWidth, width);

				FRect childRect { current.x, current.y, width, height };
				child->Arrange(childRect);

				current.y += height;
				visibleInLine++;
			}
		}
		else
		{
			FPoint current { rect.x, rect.y };
			float rowWidth = 0;
			float rowHeight = 0;
			size_t visibleInRow = 0;

			for(auto& child : Children)
			{
				if(!child->IsVisible())
				{
					continue;
				}

				auto [width, height] = child->GetDesiredSize();

				if(rowWidth + width > rect.w)
				{
					current.y += rowHeight;

					if(visibleInRow > 0)
					{
						current.y += _verticalSpacing;
					}

					current.x = rect.x;
					rowWidth = 0;
					rowHeight = 0;
					visibleInRow = 0;
				}

				if(rowWidth > 0)
				{
					current.x += _horizontalSpacing;
					rowWidth += _horizontalSpacing;
				}

				rowWidth += width;
				rowHeight = std::max(rowHeight, height);

				FRect childRect { current.x, current.y, width, height };
				child->Arrange(childRect);

				current.x += width;
				visibleInRow++;
			}
		}
	}
}