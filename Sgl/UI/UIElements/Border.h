#pragma once

#include "../ContentUIElement.h"

namespace Sgl
{
	class Border: public ContentUIElement
	{
	public:
		ObservableProperty<size_t> BorderWidth;
		ObservableProperty<Color> BorderColor;
	public:
		Border();
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void Render(RenderContext context) const final;
	};
}