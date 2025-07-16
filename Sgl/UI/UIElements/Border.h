#pragma once

#include "../ContentUIElement.h"

namespace Sgl
{
	class Border: public ContentUIElement
	{
	public:
		StylyableProperty<size_t> BorderWidth;
		StylyableProperty<Color> BorderColor;
	public:
		static std::shared_ptr<Border> New() { return std::make_shared<Border>(); }

		Border();
		Border(const Border& other);
		Border(Border&& other) noexcept;

		void OnRender(RenderContext context) const final;
	};
}