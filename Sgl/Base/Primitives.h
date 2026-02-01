#pragma once

#include <SDL3/SDL_rect.h>

namespace Sgl
{
	/// <summary>
	/// Represents a 2D point with integer coordinates (x, y).
	/// </summary>
	using Point = SDL_Point;

	/// <summary>
	/// Represents a 2D point with floating-point coordinates (x, y).
	/// </summary>
	using FPoint = SDL_FPoint;

	/// <summary>
	/// Represents an axis-aligned rectangle with integer coordinates and dimensions.
	/// </summary>
	using Rect = SDL_Rect;

	/// <summary>
	/// Represents an axis-aligned rectangle with floating-point coordinates and dimensions.
	/// </summary>
	using FRect = SDL_FRect;
}