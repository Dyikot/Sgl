#pragma once

#include "../Base/Primitives.h"
#include "../Base/Size.h"
#include "Layout.h"

namespace Sgl
{
	class Layoutable;

	//! @brief Determines whether a point (x, y) lies within the bounds of a floating-point rectangle
	//! @param x X-coordinate of the point
	//! @param y Y-coordinate of the point
	//! @param rect The rectangle to test against
	//! @return True if the point is inside or on the edge of the rectangle; otherwise, false
	constexpr bool IsPointInRect(float x, float y, FRect rect) noexcept
	{
		return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	}

	//! @brief Determines whether a point (x, y) lies within the bounds of an integer rectangle
	//! @param x X-coordinate of the point
	//! @param y Y-coordinate of the point
	//! @param rect The rectangle to test against
	//! @return True if the point is inside or on the edge of the rectangle; otherwise, false
	constexpr bool IsPointInRect(int x, int y, Rect rect) noexcept
	{
		return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	}

	//! @brief Shrinks a size by subtracting the horizontal and vertical thickness
	//! @param size The original size to shrink
	//! @param thickness The thickness to subtract from the size
	//! @return The new shrunk size
	constexpr FSize Shrink(FSize size, Thickness thickness) noexcept
	{
		return FSize
		{
			.Width = size.Width - thickness.Left - thickness.Right,
			.Height = size.Height - thickness.Top - thickness.Bottom
		};
	}

	//! @brief Shrinks a rectangle by applying the thickness as inward padding (offsets position and reduces dimensions)
	//! @param rect The original rectangle to shrink
	//! @param thickness The thickness to apply as inward padding
	//! @return The new shrunk rectangle
	constexpr FRect Shrink(FRect rect, Thickness thickness) noexcept
	{
		return FRect
		{
			.x = rect.x + thickness.Left,
			.y = rect.y + thickness.Top,
			.w = rect.w - thickness.Left - thickness.Right,
			.h = rect.h - thickness.Top - thickness.Bottom
		};
	}

	//! @brief Expands a size by adding the horizontal and vertical thickness
	//! @param size The original size to expand
	//! @param thickness The thickness to add to the size
	//! @return The new expanded size
	constexpr FSize Expand(FSize size, Thickness thickness) noexcept
	{
		return FSize
		{
			.Width = size.Width + thickness.Left + thickness.Right,
			.Height = size.Height + thickness.Top + thickness.Bottom
		};
	}

	//! @brief Expands a rectangle by applying the thickness as outward margin (offsets position outwards and increases dimensions)
	//! @param rect The original rectangle to expand
	//! @param thickness The thickness to apply as outward margin
	//! @return The new expanded rectangle
	constexpr FRect Expand(FRect rect, Thickness thickness) noexcept
	{
		return FRect
		{
			.x = rect.x - thickness.Left,
			.y = rect.y - thickness.Top,
			.w = rect.w + thickness.Left + thickness.Right,
			.h = rect.h + thickness.Top + thickness.Bottom
		};
	}

	//! @brief Measures a child layoutable element within the available space minus the specified padding
	//! @param child The child element to measure
	//! @param availableSize The total available size for the parent
	//! @param padding The padding to subtract from available size and add to the result
	//! @return The desired size of the child plus the padding, or just the padding size if the child is null
	FSize MeasureChild(Layoutable* child, FSize availableSize, Thickness padding);

	//! @brief Arranges a child layoutable element within the specified rectangle minus the specified padding
	//! @param child The child element to arrange
	//! @param rect The total available rectangle for the parent
	//! @param padding The padding to subtract from the rect
	void ArrangeChild(Layoutable* child, FRect rect, Thickness padding);
}