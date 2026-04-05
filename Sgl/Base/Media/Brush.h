#pragma once

#include <variant>
#include "Color.h"
#include "ImagePath.h"

namespace Sgl
{	
	/// <summary>
	/// Represents a drawing brush that can be either a solid color or an image.
	/// </summary>
	using Brush = std::variant<Color, ImagePath>;

	inline bool operator==(const Brush& brush, Color color)
	{
		return brush.index() == 0 && std::get<Color>(brush) == color;
	}

	inline bool operator==(const Brush& brush, const ImagePath& path)
	{
		return brush.index() == 1 && std::get<ImagePath>(brush) == path;
	}
}