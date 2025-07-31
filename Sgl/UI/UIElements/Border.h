#pragma once

#include "../ContentUIElement.h"

namespace Sgl
{
	class Border: public ContentUIElement
	{
	public:
		BindableProperty<size_t> BorderWidth;
		BindableProperty<Color> BorderColor;
	public:
		Border();
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void Render(RenderContext context) const final;
	};
}