#include "Grid.h"

#include <algorithm>
#include "../../Base/Views.h"

namespace Sgl::UIElements
{
	struct Context
	{
		uint32_t Column;
		uint32_t Row;
		uint32_t ColumnSpan;
		uint32_t RowSpan;
	};

	LayoutProperty<uint32_t> Grid::ColumnProperty =
	{
		[](Layoutable& element, uint32_t value)
		{
			if(element.SetProperty(ColumnProperty, element.GetLayoutContext<Context>().Column, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Column; }
	};

	LayoutProperty<uint32_t> Grid::RowProperty =
	{
		[](Layoutable& element, uint32_t value)
		{
			if(element.SetProperty(RowProperty, element.GetLayoutContext<Context>().Row, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().Row; }
	};

	LayoutProperty<uint32_t> Grid::ColumnSpanProperty =
	{
		[](Layoutable& element, uint32_t value)
		{
			if(element.SetProperty(ColumnSpanProperty, element.GetLayoutContext<Context>().ColumnSpan, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().ColumnSpan; }
	};

	LayoutProperty<uint32_t> Grid::RowSpanProperty =
	{
		[](Layoutable& element, uint32_t value)
		{
			if(element.SetProperty(RowSpanProperty, element.GetLayoutContext<Context>().RowSpan, value))
			{
				element.InvalidateMeasure();
			}
		},
		[](Layoutable& element) { return element.GetLayoutContext<Context>().RowSpan; }
	};

	Grid::Grid(Grid&& other) noexcept:
		Panel(std::move(other)),
		_columnDefenitionsStr(std::move(other._columnDefenitionsStr)),
		_rowDefenitionsStr(std::move(other._rowDefenitionsStr)),
		_rowDefenitions(std::move(other._rowDefenitions)),
		_columnDefenitions(std::move(other._columnDefenitions)),
		_measuredWidth(std::move(other._measuredWidth)),
		_measuredHeight(std::move(other._measuredHeight))
	{}

	void Grid::SetColumn(const Ref<UIElement>& element, uint32_t value)
	{
		if(element->SetProperty(ColumnProperty, element->GetLayoutContext<Context>().Column, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetColumn(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Column;
	}

	void Grid::SetRow(const Ref<UIElement>& element, uint32_t value)
	{
		if(element->SetProperty(RowProperty, element->GetLayoutContext<Context>().Row, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetRow(const Ref<UIElement>&element)
	{
		return element->GetLayoutContext<Context>().Row;
	}

	void Grid::SetColumnSpan(const Ref<UIElement>& element, uint32_t value)
	{
		if(element->SetProperty(ColumnSpanProperty, element->GetLayoutContext<Context>().ColumnSpan, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetColumnSpan(const Ref<UIElement>&element)
	{
		return element->GetLayoutContext<Context>().ColumnSpan;
	}

	void Grid::SetRowSpan(const Ref<UIElement>& element, uint32_t value)
	{
		if(element->SetProperty(RowSpanProperty, element->GetLayoutContext<Context>().RowSpan, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetRowSpan(const Ref<UIElement>&element)
	{
		return element->GetLayoutContext<Context>().RowSpan;
	}

	static std::vector<ColumnDefenition> ParseDefenition(std::string_view defenition)
	{
		std::vector<ColumnDefenition> defenitions;

		for(auto token : defenition | std::views::split(' ') | Views::Cast<std::string_view>())
		{
			const auto size = token.size();

			if(size == 0)
			{
				continue;
			}

			if(token == "auto")
			{
				defenitions.emplace_back(SizingMode::Auto, 0.f);
			}
			else if(token.back() == '*')
			{
				if(size == 1)
				{
					defenitions.emplace_back(SizingMode::Proportional, 1.f);
				}
				else
				{
					float value;
					auto begin = token.data();
					auto end = token.data() + token.size() - 1;
					auto [_, error] = std::from_chars(begin, end, value);

					if(error == std::errc())
					{
						defenitions.emplace_back(SizingMode::Proportional, value);
					}
				}
			}
			else
			{
				float value;
				auto begin = token.data();
				auto end = token.data() + token.size();
				auto [_, error] = std::from_chars(begin, end, value);

				if(error == std::errc())
				{
					defenitions.emplace_back(SizingMode::Fixed, value);
				}
			}
		}

		return defenitions;
	}

	void Grid::SetColumnDefenitions(const std::string& value, ValueSource source)
	{
		if(SetProperty(ColumnDefenitionsProperty, _columnDefenitionsStr, value, _columnDefenitionsValueSource, source))
		{
			InvalidateMeasure();
			_columnDefenitions = ParseDefenition(_columnDefenitionsStr);
		}
	}

	void Grid::SetRowDefenitions(const std::string& value, ValueSource source)
	{
		if(SetProperty(RowDefenitionsProperty, _rowDefenitionsStr, value, _rowDefenitionsValueSource, source))
		{
			InvalidateMeasure();
			_rowDefenitions = ParseDefenition(_rowDefenitionsStr);
		}
	}
	
	FSize Grid::MeasureContent(FSize availableSize)
	{
		FSize size {};

		if(_rowDefenitions.empty() || _columnDefenitions.empty())
		{
			_measuredWidth.clear();
			_measuredHeight.clear();
			return size;
		}

		const auto rows = _rowDefenitions.size();
		const auto columns = _columnDefenitions.size();
		const auto cells = rows * columns;

		// Mearuse children and save desired width/height
		std::vector<float> desiredHeight(rows);
		std::vector<float> desiredWidth(columns);
		for(auto& child : Children)
		{
			auto& [column, row, columnSpan, rowShan] = child->GetLayoutContext<Context>();

			child->Measure(availableSize);
			auto [width, height] = child->GetDesiredSize();

			if(row >= rows)
			{
				throw Exception("Row index exceeds number of rows - {}.", rows);
			}

			if(column >= columns)
			{
				throw Exception("Column index exceeds number of columns - {}.", rows);
			}

			desiredHeight[row] = height;
			desiredWidth[column] = width;
		}

		std::vector<float> actualHeight(rows);
		std::vector<float> actualWidth(columns);

		// Set the height for rows
		for(size_t i = 0; i < rows; i++)
		{
			switch(_rowDefenitions[i].Mode)
			{
				case SizingMode::Auto:
					actualHeight[i] = std::max(desiredHeight[i], _rowDefenitions[i].Value);
					break;

				case SizingMode::Fixed:
					actualHeight[i] = _rowDefenitions[i].Value;
					break;

				case SizingMode::Proportional:
					break;
			}
		}

		// Set the width for columns
		for(size_t i = 0; i < columns; i++)
		{
			switch(_columnDefenitions[i].Mode)
			{
				case SizingMode::Auto:
					actualWidth[i] = std::max(desiredWidth[i], _columnDefenitions[i].Value);
					break;

				case SizingMode::Fixed:
					actualWidth[i] = _columnDefenitions[i].Value;
					break;

				case SizingMode::Proportional:
					break;
			}
		}

		// Calculate proportional width/height		
		float usedWidth = 0;
		float usedHeight = 0;
		float proportionalWidth = 0;
		float proportionalHeight = 0;

		for(size_t i = 0; i < rows; ++i)
		{
			if(_rowDefenitions[i].Mode == SizingMode::Proportional)
			{
				proportionalHeight += _rowDefenitions[i].Value;
			}
			else
			{
				usedHeight += actualHeight[i];
			}
		}

		for(size_t i = 0; i < columns; ++i)
		{
			if(_columnDefenitions[i].Mode == SizingMode::Proportional)
			{
				proportionalWidth += _columnDefenitions[i].Value;
			}
			else
			{
				usedWidth += actualWidth[i];
			}
		}		

		float remainingWidth = std::max(0.f, availableSize.Width - usedWidth);
		float remainingHeight = std::max(0.f, availableSize.Height - usedHeight);

		if(proportionalHeight > 0.f)
		{
			for(size_t i = 0; i < rows; i++)
			{
				if(_rowDefenitions[i].Mode == SizingMode::Proportional)
				{
					actualHeight[i] = _rowDefenitions[i].Value / proportionalHeight * remainingHeight;
				}
			}
		}

		if(proportionalWidth > 0.f)
		{
			for(size_t i = 0; i < columns; i++)
			{
				if(_columnDefenitions[i].Mode == SizingMode::Proportional)
				{
					actualWidth[i] = _columnDefenitions[i].Value / proportionalWidth * remainingWidth;
				}
			}
		}

		size = 
		{
			.Width = std::reduce(actualWidth.begin(), actualWidth.end()),
			.Height = std::reduce(actualHeight.begin(), actualHeight.end())
		};

		_measuredWidth.swap(actualWidth);
		_measuredHeight.swap(actualHeight);

		return size;
	}

	void Grid::ArrangeContent(FRect rect)
	{
		if(_measuredWidth.empty() || _measuredHeight.empty())
		{
			return;
		}

		const uint32_t rows = _rowDefenitions.size();
		const uint32_t columns = _columnDefenitions.size();

		// Calculating column and row positions
		std::vector<float> rowY(rows + 1, rect.y);
		std::vector<float> columnX(columns + 1, rect.x);

		for(size_t i = 0; i < rows; ++i)
		{
			rowY[i + 1] = rowY[i] + _measuredHeight[i];
		}

		for(size_t i = 0; i < columns; ++i)
		{
			columnX[i + 1] = columnX[i] + _measuredWidth[i];
		}

		// Arrange children
		for(auto& child : Children)
		{
			auto [column, row, columnSpan, rowSpan] = child->GetLayoutContext<Context>();

			row = std::min(row, rows - 1);
			column = std::min(column, columns - 1);
			rowSpan = std::clamp(rowSpan, 1u, rows - row);
			columnSpan = std::clamp(columnSpan, 1u, columns - column);

			FRect childRect =
			{
				.x = columnX[column],
				.y = rowY[row],
				.w = columnX[column + columnSpan] - columnX[column],
				.h = rowY[row + rowSpan] - rowY[row]
			};

			child->Arrange(childRect);
		}
	}
}

