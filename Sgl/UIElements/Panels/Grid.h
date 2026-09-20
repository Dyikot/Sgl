#pragma once

#include "Panel.h"

namespace Sgl::UIElements
{
	//! @brief Specifies the sizing mode for a row or column in a layout
	enum class SizingMode
	{
		Auto,        // Size is determined automatically based on content
		Fixed,       // Size is explicitly specified in absolute units
		Proportional // Size is distributed proportionally based on a weight value
	};

	//! @brief Defines the sizing properties for a column in a grid layout
	struct ColumnDefinition
	{
		SizingMode Mode; // The sizing mode applied to this column
		float Value;     // The value associated with the sizing mode
	};

	//! @brief Defines the sizing properties for a row in a grid layout
	using RowDefinition = ColumnDefinition;

	class Grid : public Panel
	{		
	public:
		Grid();

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

		static inline StyleableProperty ColumnDefinitionsProperty { &SetColumnDefinitions, &GetColumnDefinitions };
		static inline StyleableProperty RowDefinitionsProperty { &SetRowDefinitions, &GetRowDefinitions };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
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