#pragma once

#include <variant>
#include "Color.h"
#include "../../Render/Texture.h"

namespace Sgl
{	
	using Brush = std::variant<Color, Texture>;

	constexpr bool operator==(const Brush& brush, Color color)
	{
		return brush.index() == 0 && std::get<Color>(brush) == color;
	}

	constexpr bool operator==(const Brush& brush, const Texture& texture)
	{
		return brush.index() == 1 && std::get<Texture>(brush) == texture;
	}
}