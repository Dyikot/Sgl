#pragma once

#include "../Base/Primitives.h"

namespace Sgl
{
	/// <summary>
	/// Provides utility functions for common layout operations.
	/// </summary>
	class LayoutHelper
	{
	public:
		/// <summary>
		/// Determines whether a point (x, y) lies within the bounds of a floating-point rectangle.
		/// </summary>
		/// <param name="x"> - x-coordinate of the point.</param>
		/// <param name="y"> - y-coordinate of the point.</param>
		/// <param name="rect"> - the rectangle to test against.</param>
		/// <returns>True if the point is inside or on the edge of the rectangle; otherwise, false.</returns>
		static bool IsPointInRect(float x, float y, FRect rect) noexcept
		{
			return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
		}

		/// <summary>
		/// Determines whether a point (x, y) lies within the bounds of an integer rectangle.
		/// </summary>
		/// <param name="x"> - x-coordinate of the point.</param>
		/// <param name="y"> - y-coordinate of the point.</param>
		/// <param name="rect"> - the rectangle to test against.</param>
		/// <returns>True if the point is inside or on the edge of the rectangle; otherwise, false.</returns>
		static bool IsPointInRect(int x, int y, Rect rect) noexcept
		{
			return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
		}
	};
}