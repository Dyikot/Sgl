#pragma once

#include <SDL3/SDL_rect.h>

namespace Sgl
{
	//! @brief Represents a 2D point with integer coordinates (x, y)
	using Point = SDL_Point;
		
	//! @brief Represents a 2D point with floating-point coordinates (x, y)
	using FPoint = SDL_FPoint;
		
	//! @brief Represents an axis-aligned rectangle with integer coordinates and dimensions
	using Rect = SDL_Rect;
		
	//! @brief Represents an axis-aligned rectangle with floating-point coordinates and dimensions
	using FRect = SDL_FRect;

	//! @brief Converts FRect to Rect
	//! @param rect Source rectable
	//! @return Target rectable
	constexpr Rect ToRect(FRect rect)
	{
		return Rect(rect.x, rect.y, rect.w, rect.h);
	}
}