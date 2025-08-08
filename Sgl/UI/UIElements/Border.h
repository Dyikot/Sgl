#pragma once

#include "../ContentUIElement.h"

namespace Sgl::UIElements
{
	class Border: public ContentUIElement
	{
	private:
		size_t _borderWidth;
		Color _borderColor;
	public:
		Border();
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void SetBorderWidth(size_t value)
		{ 
			SetProperty(BorderWidthProperty, _borderWidth, value);
		}

		size_t GetBorderWidth() const
		{ 
			return _borderWidth;
		}

		void SetBorderColor(Color value)
		{ 
			SetProperty(BorderColorProperty, _borderColor, value);
		}

		Color GetBorderColor() const
		{ 
			return _borderColor; 
		}

		void Render(RenderContext context) final;
	public:
		static inline BindableProperty<Border, size_t> BorderWidthProperty =
			BindableProperty<Border, size_t>(&SetBorderWidth, 1);

		static inline BindableProperty<Border, Color> BorderColorProperty =
			BindableProperty<Border, Color>(&SetBorderColor, Colors::Black);
	};
}