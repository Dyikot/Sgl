#include "UniformGrid.h"

namespace Sgl::UIElements
{
	UniformGrid::UniformGrid(UniformGrid&& other) noexcept:
        Panel(std::move(other)),
        _rows(other._rows),
        _columns(other._columns),
        _rowSpacing(other._rowSpacing),
        _columnSpacing(other._columnSpacing)
	{}

	void UniformGrid::SetRows(uint32_t value, ValueSource source)
	{
		if(SetProperty(RowsProperty, _rows, value, _rowsValueSource, source))
		{
			InvalidateMeasure();
		}
	}

	void UniformGrid::SetColumns(uint32_t value, ValueSource source)
	{
		if(SetProperty(ColumnsProperty, _columns, value, _columnsValueSource, source))
		{
			InvalidateMeasure();
		}
	}

	void UniformGrid::SetRowSpacing(float value, ValueSource source)
	{
		if(SetProperty(RowSpacingProperty, _rowSpacing, value, _rowSpacingValueSource, source))
		{
			InvalidateMeasure();
		}
	}

	void UniformGrid::SetColumnSpacing(float value, ValueSource source)
	{
		if(SetProperty(ColumnSpacingProperty, _columnSpacing, value, _columnSpacingValueSource, source))
		{
			InvalidateMeasure();
		}
	}

    FSize UniformGrid::MeasureContent(FSize availableSize)
    {
        FSize size = {};

        size_t visibleChildren = 0;
        for(const auto& child : Children)
        {
            if(child->IsVisible())
            {
                visibleChildren++;
            }
        }

        if(visibleChildren == 0)
        {
            return size;
        }

        uint32_t rows = _rows;
        uint32_t columns = _columns;

        if(rows == 0 && columns == 0)
        {
            columns = static_cast<uint32_t>(std::ceil(std::sqrt(visibleChildren)));
            rows = columns;
        }
        else if(rows == 0)
        {
            rows = static_cast<uint32_t>(std::ceil(static_cast<float>(visibleChildren) / columns));
        }
        else if(columns == 0)
        {
            columns = static_cast<uint32_t>(std::ceil(static_cast<float>(visibleChildren) / rows));
        }

        float totalColumnSpacing = (columns > 1) ? _columnSpacing * (columns - 1) : 0.f;
        float totalRowSpacing = (rows > 1) ? _rowSpacing * (rows - 1) : 0.f;

        FSize contentSize =
        {
            .Width = std::max(0.f, availableSize.Width - totalColumnSpacing),
            .Height = std::max(0.f, availableSize.Height - totalRowSpacing)
        };

        FSize cellSize =
        {
            .Width = contentSize.Width / columns,
            .Height = contentSize.Height / rows
        };

        FSize maxDesiredSize = cellSize;
        for(const auto& child : Children)
        {
            if(!child->IsVisible())
            {
                continue;
            }

            child->Measure(cellSize);
            auto [width, height] = child->GetDesiredSize();
            maxDesiredSize.Width = std::max(maxDesiredSize.Width, width);
            maxDesiredSize.Height = std::max(maxDesiredSize.Height, height);
        }

        FSize finalCellSize = 
        {
            .Width = std::min(cellSize.Width, maxDesiredSize.Width),
            .Height = std::min(cellSize.Height, maxDesiredSize.Height)
        };

        size.Width = finalCellSize.Width * columns + totalColumnSpacing;
        size.Height = finalCellSize.Height * rows + totalRowSpacing;

        return size;
    }

    void UniformGrid::ArrangeContent(FRect rect)
    {
        size_t visibleChildren = 0;
        for(const auto& child : Children)
        {
            if(child->IsVisible())
            {
                visibleChildren++;
            }
        }

        if(visibleChildren == 0)
        {
            return;
        }

        uint32_t rows = _rows;
        uint32_t columns = _columns;

        if(rows == 0 && columns == 0)
        {
            columns = static_cast<uint32_t>(std::ceil(std::sqrt(visibleChildren)));
            rows = columns;
        }
        else if(rows == 0)
        {
            rows = static_cast<uint32_t>(std::ceil(static_cast<float>(visibleChildren) / columns));
        }
        else if(columns == 0)
        {
            columns = static_cast<uint32_t>(std::ceil(static_cast<float>(visibleChildren) / rows));
        }

        float totalColumnSpacing = (columns > 1) ? _columnSpacing * (columns - 1) : 0.f;
        float totalRowSpacing = (rows > 1) ? _rowSpacing * (rows - 1) : 0.f;

        FSize contentSize = 
        {
            .Width = std::max(0.f, rect.w - totalColumnSpacing),
            .Height = std::max(0.f, rect.h - totalRowSpacing)
        };

        FSize cellSize =
        {
            .Width = contentSize.Width / columns,
            .Height = contentSize.Height / rows
        };

        uint32_t index = 0;
        uint32_t cellCount = rows * columns;
        for(const auto& child : Children)
        {
            if(!child->IsVisible())
            {
                continue;
            }

            uint32_t row = index / columns;
            uint32_t col = index % columns;

            FRect childRect = 
            {
                .x = rect.x + col * (cellSize.Width + _columnSpacing),
                .y = rect.y + row * (cellSize.Height + _rowSpacing),
                .w = cellSize.Width,
                .h = cellSize.Height
            };

            child->Arrange(childRect);
            index++;

            if(index >= cellCount)
            {
                break;
            }
        }
    }
}

