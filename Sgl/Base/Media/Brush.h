#pragma once

#include <variant>
#include "Color.h"
#include "ImageSource.h"

namespace Sgl
{	
	/// <summary>
	/// Represents a drawing brush that can be either a solid color or an image source.
	/// </summary>
	using Brush = std::variant<Color, ImageSource>;

	inline bool operator==(Brush brush, Color color)
	{
		return brush.index() == 0 && std::get<Color>(brush) == color;
	}

	inline bool operator==(Brush brush, const ImageSource& imageSource)
	{
		return brush.index() == 1 && std::get<ImageSource>(brush) == imageSource;
	}
}