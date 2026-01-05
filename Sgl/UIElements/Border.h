#pragma once

#include "../UIElement/ContentUIElement.h"

namespace Sgl::UIElements
{
	class Border : public ContentUIElement
	{
	private:
		size_t _borderWidth = 1;
		Color _borderColor = Colors::Black;

		ValueSource _borderWidthSource {};
		ValueSource _borderColorSource {};
	public:
		Border() = default;
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void SetBorderWidth(size_t value);
		void SetBorderWidth(size_t value, ValueSource source);
		size_t GetBorderWidth() const { return _borderWidth; }

		void SetBorderColor(Color value);
		void SetBorderColor(Color value, ValueSource source);
		Color GetBorderColor() const { return _borderColor; }

		void Render(RenderContext context) final;
	public:
		static inline StyleableProperty BorderWidthProperty { &SetBorderWidth, &GetBorderWidth };
		static inline StyleableProperty BorderColorProperty { &SetBorderColor, &GetBorderColor };
	};
}