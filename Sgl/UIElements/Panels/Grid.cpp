#include "Grid.h"

#include <algorithm>
#include <numeric>
#include <ranges>

namespace
{
	struct Context
	{
		uint32_t Column;
		uint32_t Row;
		uint32_t ColumnSpan;
		uint32_t RowSpan;
	};

	bool SetField(uint32_t& field, uint32_t value)
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
	Grid::Grid()
	{
		Name = "Grid";
	}

	void Grid::SetColumn(const Ref<UIElement>& element, uint32_t value)
	{
		if(SetField(element->GetLayoutContext<Context>().Column, value))
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
		if(SetField(element->GetLayoutContext<Context>().Row, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetRow(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().Row;
	}

	void Grid::SetColumnSpan(const Ref<UIElement>& element, uint32_t value)
	{
		if(SetField(element->GetLayoutContext<Context>().ColumnSpan, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetColumnSpan(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().ColumnSpan;
	}

	void Grid::SetRowSpan(const Ref<UIElement>& element, uint32_t value)
	{
		if(SetField(element->GetLayoutContext<Context>().RowSpan, value))
		{
			element->InvalidateMeasure();
		}
	}

	uint32_t Grid::GetRowSpan(const Ref<UIElement>& element)
	{
		return element->GetLayoutContext<Context>().RowSpan;
	}

	static std::vector<ColumnDefinition> ParseDefenition(std::string_view defenition)
	{
		std::vector<ColumnDefinition> defenitions;

		auto toStringView = std::views::transform([](auto&& range)
		{
			return std::string_view(range.begin(), range.end());
		});

		for(auto token : defenition | std::views::split(' ') | toStringView)
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

	void Grid::SetColumnDefinitions(const std::string& value, ValueSource source)
	{
		SetProperty(ColumnDefinitionsProperty, _columnDefinitionsStr, value, _columnDefinitionsValueSource, source);
	}

	void Grid::SetRowDefinitions(const std::string& value, ValueSource source)
	{
		SetProperty(RowDefinitionsProperty, _rowDefinitionsStr, value, _rowDefinitionsValueSource, source);
	}
	
	void Grid::OnPropertyChanged(PropertyBase& property)
	{
		Panel::OnPropertyChanged(property);

		if(property == ColumnDefinitionsProperty)
		{
			_columnDefinitions = ParseDefenition(_columnDefinitionsStr);
			InvalidateMeasure();
		}
		else if(property == RowDefinitionsProperty)
		{
			_rowDefinitions = ParseDefenition(_rowDefinitionsStr);
			InvalidateMeasure();
		}
	}

	FSize Grid::MeasureContent(FSize availableSize)
	{
		FSize size {};

		if(_rowDefinitions.empty() || _columnDefinitions.empty())
		{
			_measuredWidth.clear();
			_measuredHeight.clear();
			return size;
		}

		const auto rows = _rowDefinitions.size();
		const auto columns = _columnDefinitions.size();
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

			if(height > desiredHeight[row])
			{
				desiredHeight[row] = height;
			}

			if(width > desiredWidth[column])
			{
				desiredWidth[column] = width;
			}
		}

		std::vector<float> actualHeight(rows);
		std::vector<float> actualWidth(columns);

		// Set the height for rows
		for(size_t i = 0; i < rows; i++)
		{
			switch(_rowDefinitions[i].Mode)
			{
				case SizingMode::Auto:
					actualHeight[i] = std::max(desiredHeight[i], _rowDefinitions[i].Value);
					break;

				case SizingMode::Fixed:
					actualHeight[i] = _rowDefinitions[i].Value;
					break;

				case SizingMode::Proportional:
					break;
			}
		}

		// Set the width for columns
		for(size_t i = 0; i < columns; i++)
		{
			switch(_columnDefinitions[i].Mode)
			{
				case SizingMode::Auto:
					actualWidth[i] = std::max(desiredWidth[i], _columnDefinitions[i].Value);
					break;

				case SizingMode::Fixed:
					actualWidth[i] = _columnDefinitions[i].Value;
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
			if(_rowDefinitions[i].Mode == SizingMode::Proportional)
			{
				proportionalHeight += _rowDefinitions[i].Value;
			}
			else
			{
				usedHeight += actualHeight[i];
			}
		}

		for(size_t i = 0; i < columns; ++i)
		{
			if(_columnDefinitions[i].Mode == SizingMode::Proportional)
			{
				proportionalWidth += _columnDefinitions[i].Value;
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
				if(_rowDefinitions[i].Mode == SizingMode::Proportional)
				{
					actualHeight[i] = _rowDefinitions[i].Value / proportionalHeight * remainingHeight;
				}
			}
		}

		if(proportionalWidth > 0.f)
		{
			for(size_t i = 0; i < columns; i++)
			{
				if(_columnDefinitions[i].Mode == SizingMode::Proportional)
				{
					actualWidth[i] = _columnDefinitions[i].Value / proportionalWidth * remainingWidth;
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

		const uint32_t rows = _rowDefinitions.size();
		const uint32_t columns = _columnDefinitions.size();

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

