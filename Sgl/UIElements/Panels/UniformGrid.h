#pragma once

#include "Panel.h"

namespace Sgl::UIElements
{
	class UniformGrid : public Panel
	{
	public:
		UniformGrid() = default;
		UniformGrid(const UniformGrid&) = delete;
		UniformGrid(UniformGrid&& other) noexcept;

		void SetRows(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetRows() const { return _rows; }

		void SetColumns(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetColumns() const { return _columns; }

		void SetRowSpacing(float value, ValueSource source = ValueSource::Local);
		float GetRowSpacing() const { return _rowSpacing; }

		void SetColumnSpacing(float value, ValueSource source = ValueSource::Local);
		float GetColumnSpacing() const { return _columnSpacing; }

		static inline StyleableProperty RowsProperty { &SetRows, &GetRows };
		static inline StyleableProperty ColumnsProperty { &SetColumns, &GetColumns };
		static inline StyleableProperty RowSpacingProperty { &SetRowSpacing, &GetRowSpacing };
		static inline StyleableProperty ColumnSpacingProperty { &SetColumnSpacing, &GetColumnSpacing };
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		uint32_t _rows = 0;
		uint32_t _columns = 0;
		float _rowSpacing = 0;
		float _columnSpacing = 0;

		ValueSource _rowsValueSource {};
		ValueSource _columnsValueSource {};
		ValueSource _rowSpacingValueSource {};
		ValueSource _columnSpacingValueSource {};
	};
}