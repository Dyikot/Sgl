#pragma once

#include "Panel.h"
#include "../../Data/LayoutProperty.h"

namespace Sgl::UIElements
{
	enum class SizingMode
	{
		Auto,
		Fixed,
		Proportional
	};

	struct ColumnDefenition
	{
		SizingMode Mode;
		float Value;
	};

	using RowDefenition = ColumnDefenition;

	class Grid : public Panel
	{		
	public:
		static void SetColumn(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetColumn(const Ref<UIElement>& element);

		static void SetRow(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetRow(const Ref<UIElement>& element);

		static void SetColumnSpan(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetColumnSpan(const Ref<UIElement>& element);

		static void SetRowSpan(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetRowSpan(const Ref<UIElement>& element);

		void SetColumnDefenitions(const std::string& value, ValueSource source = ValueSource::Local);
		const std::string& GetColumnDefenitions() const { return _columnDefenitionsStr; }

		void SetRowDefenitions(const std::string& value, ValueSource source = ValueSource::Local);
		const std::string& GetRowDefenitions() const { return _rowDefenitionsStr; }

		static LayoutProperty<uint32_t> ColumnProperty;
		static LayoutProperty<uint32_t> RowProperty;
		static LayoutProperty<uint32_t> ColumnSpanProperty;
		static LayoutProperty<uint32_t> RowSpanProperty;
		static inline StyleableProperty ColumnsDefenitionProperty { &SetColumnDefenitions, &GetColumnDefenitions };
		static inline StyleableProperty RowsDefenitionProperty { &SetRowDefenitions, &GetRowDefenitions };
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		std::string _columnDefenitionsStr;
		std::string _rowDefenitionsStr;
		std::vector<RowDefenition> _rowDefenitions;
		std::vector<ColumnDefenition> _columnDefenitions;
		std::vector<float> _measuredWidth;
		std::vector<float> _measuredHeight;

		ValueSource _columnDefenitionsValueSource {};
		ValueSource _rowDefenitionsValueSource {};
	};
}