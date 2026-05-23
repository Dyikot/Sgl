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

	struct ColumnDefinition
	{
		SizingMode Mode;
		float Value;
	};

	using RowDefinition = ColumnDefinition;

	class Grid : public Panel
	{		
	public:
		Grid() = default;
		Grid(const Grid&) = delete;
		Grid(Grid&& other) noexcept;

		static void SetColumn(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetColumn(const Ref<UIElement>& element);

		static void SetRow(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetRow(const Ref<UIElement>& element);

		static void SetColumnSpan(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetColumnSpan(const Ref<UIElement>& element);

		static void SetRowSpan(const Ref<UIElement>& element, uint32_t value);
		static uint32_t GetRowSpan(const Ref<UIElement>& element);

		void SetColumnDefinitions(const std::string& value, ValueSource source = ValueSource::Local);
		const std::string& GetColumnDefinitions() const { return _columnDefinitionsStr; }

		void SetRowDefinitions(const std::string& value, ValueSource source = ValueSource::Local);
		const std::string& GetRowDefinitions() const { return _rowDefinitionsStr; }

		static LayoutProperty<uint32_t> ColumnProperty;
		static LayoutProperty<uint32_t> RowProperty;
		static LayoutProperty<uint32_t> ColumnSpanProperty;
		static LayoutProperty<uint32_t> RowSpanProperty;
		static inline StyleableProperty ColumnDefinitionsProperty { &SetColumnDefinitions, &GetColumnDefinitions };
		static inline StyleableProperty RowDefinitionsProperty { &SetRowDefinitions, &GetRowDefinitions };
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		std::string _columnDefinitionsStr;
		std::string _rowDefinitionsStr;
		std::vector<RowDefinition> _rowDefinitions;
		std::vector<ColumnDefinition> _columnDefinitions;
		std::vector<float> _measuredWidth;
		std::vector<float> _measuredHeight;

		ValueSource _columnDefinitionsValueSource {};
		ValueSource _rowDefinitionsValueSource {};
	};
}