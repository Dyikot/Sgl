#pragma once
#include "../Base/Primitives.h"
#include "../Base/Size.h"
#include "Layout.h"

namespace Sgl
{
	class Layoutable;

	/// <summary>
	/// Determines whether a point (x, y) lies within the bounds of a floating-point rectangle.
	/// </summary>
	/// <param name="x"> - x-coordinate of the point.</param>
	/// <param name="y"> - y-coordinate of the point.</param>
	/// <param name="rect"> - the rectangle to test against.</param>
	/// <returns>True if the point is inside or on the edge of the rectangle; otherwise, false.</returns>
	constexpr bool IsPointInRect(float x, float y, FRect rect) noexcept
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
	constexpr bool IsPointInRect(int x, int y, Rect rect) noexcept
	{
		return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	}

	/// <summary>
	/// Shrinks a size by subtracting the horizontal and vertical thickness.
	/// </summary>
	/// <param name="size"> - the original size to shrink.</param>
	/// <param name="thickness"> - the thickness to subtract from the size.</param>
	/// <returns>The new shrunk size.</returns>
	constexpr FSize Shrink(FSize size, Thickness thickness) noexcept
	{
		return FSize
		{
			.Width = size.Width - thickness.Left - thickness.Right,
			.Height = size.Height - thickness.Top - thickness.Bottom
		};
	}

	/// <summary>
	/// Shrinks a rectangle by applying the thickness as inward padding (offsets position and reduces dimensions).
	/// </summary>
	/// <param name="rect"> - the original rectangle to shrink.</param>
	/// <param name="thickness"> - the thickness to apply as inward padding.</param>
	/// <returns>The new shrunk rectangle.</returns>
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

	/// <summary>
	/// Expands a size by adding the horizontal and vertical thickness.
	/// </summary>
	/// <param name="size"> - the original size to expand.</param>
	/// <param name="thickness"> - the thickness to add to the size.</param>
	/// <returns>The new expanded size.</returns>
	constexpr FSize Expand(FSize size, Thickness thickness) noexcept
	{
		return FSize
		{
			.Width = size.Width + thickness.Left + thickness.Right,
			.Height = size.Height + thickness.Top + thickness.Bottom
		};
	}

	/// <summary>
	/// Expands a rectangle by applying the thickness as outward margin (offsets position outwards and increases dimensions).
	/// </summary>
	/// <param name="rect"> - the original rectangle to expand.</param>
	/// <param name="thickness"> - the thickness to apply as outward margin.</param>
	/// <returns>The new expanded rectangle.</returns>
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

	/// <summary>
	/// Measures a child layoutable element within the available space minus the specified padding. 
	/// Returns the child's desired size expanded by the padding.
	/// </summary>
	/// <param name="child"> - the child element to measure.</param>
	/// <param name="availableSize"> - the total available size for the parent.</param>
	/// <param name="padding"> - the padding to subtract from available size and add to the result.</param>
	/// <returns>The desired size of the child plus the padding, or just the padding size if the child is null.</returns>
	FSize MeasureChild(Layoutable* child, FSize availableSize, Thickness padding);

	/// <summary>
	/// Arranges a child layoutable element within the specified rectangle minus the specified padding. 
	/// Clamps the resulting dimensions to zero if the padding exceeds the rect size.
	/// </summary>
	/// <param name="child"> - the child element to arrange.</param>
	/// <param name="rect"> - the total available rectangle for the parent.</param>
	/// <param name="padding"> - the padding to subtract from the rect.</param>
	void ArrangeChild(Layoutable* child, FRect rect, Thickness padding);
}