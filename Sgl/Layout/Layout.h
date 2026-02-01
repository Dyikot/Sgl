#pragma once

namespace Sgl
{
	/// <summary>
	/// Defines how a control aligns itself horizontally in its parent control.
	/// </summary>
	enum class HorizontalAlignment
	{
		Center,		// The control centers itself in the parent control.
		Left,		// The control aligns itself to the left of the parent control.
		Right,		// The control aligns itself to the right of the parent control.
		Stretch		// The control stretches to fill the width of the parent control.
	};

	/// <summary>
	/// Defines how a control aligns itself vertically in its parent control.
	/// </summary>
	enum class VerticalAlignment
	{
		Bottom,		// The control aligns itself to the bottom of the parent control.
		Center,		// The control centers itself within the parent control.
		Top,		// The control aligns itself to the top of the parent control.
		Stretch		// The control stretches to fill the height of the parent control.
	};

	/// <summary>
	/// Defines vertical or horizontal orientation.
	/// </summary>
	enum class Orientation
	{
		Horizontal = 1,
		Vertical
	};

	/// <summary>
	/// Represents the thickness of a frame or border around a rectangle.
	/// </summary>
	struct Thickness
	{
	public:
		/// <summary>
		/// Initializes a new instance of the Thickness structure with individual values for each side
		/// </summary>
		/// <param name="left">- the left thickness</param>
		/// <param name="top">- the top thickness</param>
		/// <param name="right">- the right thickness</param>
		/// <param name="bottom">- the bottom thickness</param>
		constexpr Thickness(int left, int top, int right, int bottom) noexcept:
			Left(left), Top(top), Right(right), Bottom(bottom)
		{}

		/// <summary>
		/// Initializes a new instance of the Thickness structure with horizontal and vertical values
		/// </summary>
		/// <param name="horizontal">- the horizontal thickness (applied to left and right)</param>
		/// <param name="vertical">- the vertical thickness (applied to top and bottom)</param>
		constexpr Thickness(int horizontal, int vertical) noexcept:
			Left(horizontal), Top(vertical), Right(horizontal), Bottom(vertical)
		{}

		/// <summary>
		/// Initializes a new instance of the Thickness structure with uniform thickness
		/// </summary>
		/// <param name="value">- the uniform thickness applied to all sides (default is 0)</param>
		constexpr Thickness(int value = 0) noexcept:
			Thickness(value, value, value, value)
		{}

		/// <summary>
		/// Thickness of the left side
		/// </summary>
		int Left;

		/// <summary>
		/// Thickness of the top side
		/// </summary>
		int Top;

		/// <summary>
		/// Thickness of the right side
		/// </summary>
		int Right;

		/// <summary>
		/// Thickness of the bottom side
		/// </summary>
		int Bottom;

		friend constexpr bool operator==(const Thickness&, const Thickness&) noexcept = default;

		friend constexpr Thickness operator+(const Thickness& left, const Thickness& right) noexcept
		{
			return Thickness(
				left.Left + right.Left,
				left.Top + right.Top,
				left.Right + right.Right,
				left.Bottom + right.Bottom
			);
		}

		friend constexpr Thickness operator-(const Thickness& left, const Thickness& right) noexcept
		{
			return Thickness(
				left.Left - right.Left,
				left.Top - right.Top,
				left.Right - right.Right,
				left.Bottom - right.Bottom
			);
		}
	};
}