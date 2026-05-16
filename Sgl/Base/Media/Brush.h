#pragma once

#include <variant>
#include "Color.h"
#include "SourcePath.h"

namespace Sgl
{	
	/// <summary>
	/// Represents a drawing brush that can be either a solid color or an image path.
	/// </summary>
	using Brush = std::variant<Color, SourcePath>;

	inline bool operator==(Brush brush, Color color)
	{
		return brush.index() == 0 && std::get<Color>(brush) == color;
	}

	inline bool operator==(Brush brush, SourcePath path)
	{
		return brush.index() == 1 && std::get<SourcePath>(brush) == path;
	}
}