#pragma once

#include <variant>
#include "Color.h"
#include "../../Render/Texture.h"

namespace Sgl
{	
	/// <summary>
	/// Represents a drawing brush that can be either a solid color or a texture.
	/// </summary>
	using Brush = std::variant<Color, Texture>;

	inline bool operator==(const Brush& brush, Color color)
	{
		return brush.index() == 0 && std::get<Color>(brush) == color;
	}

	inline bool operator==(const Brush& brush, const Texture& texture)
	{
		return brush.index() == 1 && std::get<Texture>(brush) == texture;
	}
}