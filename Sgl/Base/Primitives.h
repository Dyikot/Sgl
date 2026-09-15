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
}