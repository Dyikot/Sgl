#pragma once

#include "../UIElement/ContentUIElement.h"

namespace Sgl::UIElements
{
	class Border : public ContentUIElement
	{
	public:
		Border() = default;
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void SetBorderWidth(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetBorderWidth() const { return _borderWidth; }

		void SetBorderColor(Color value, ValueSource source = ValueSource::Local);
		Color GetBorderColor() const { return _borderColor; }

		void Render(RenderContext context) final;

		static inline StyleableProperty BorderWidthProperty { &SetBorderWidth, &GetBorderWidth };
		static inline StyleableProperty BorderColorProperty { &SetBorderColor, &GetBorderColor };
	protected:
		Thickness GetLayoutPadding() const override;
	private:
		uint32_t _borderWidth = 1;
		Color _borderColor = Colors::Black;

		ValueSource _borderWidthSource {};
		ValueSource _borderColorSource {};
	};
}