#pragma once

#include "../ContentUIElement.h"

namespace Sgl::UIElements
{
	class Border: public ContentUIElement
	{
	private:
		size_t _borderWidth = 1;
		Color _borderColor = Colors::Black;
	public:
		Border() = default;
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void SetBorderWidth(size_t value);
		size_t GetBorderWidth() const { return _borderWidth; }

		void SetBorderColor(Color value);
		Color GetBorderColor() const { return _borderColor; }

		void Render(RenderContext context) final;
	public:
		static inline ObservableProperty<Border, size_t> BorderWidthProperty =
			ObservableProperty<Border, size_t>(&SetBorderWidth, &GetBorderWidth);

		static inline ObservableProperty<Border, Color> BorderColorProperty =
			ObservableProperty<Border, Color>(&SetBorderColor, &GetBorderColor);
	};
}