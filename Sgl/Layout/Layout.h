#pragma once

namespace Sgl
{
	/// <summary>
	/// Defines how a control aligns itself horizontally in its parent control.
	/// </summary>
	enum class HorizontalAlignment
	{
		/// <summary>
		/// The control centers itself in the parent control.
		/// </summary>
		Center, 
		
		/// <summary>
		/// The control aligns itself to the left of the parent control.
		/// </summary>
		Left, 
		
		/// <summary>
		/// The control aligns itself to the right of the parent control.
		/// </summary>
		Right, 
		
		/// <summary>
		/// The control stretches to fill the width of the parent control.
		/// </summary>
		Stretch
	};

	/// <summary>
	/// Defines how a control aligns itself vertically in its parent control.
	/// </summary>
	enum class VerticalAlignment
	{
		/// <summary>
		/// The control aligns itself to the bottom of the parent control.
		/// </summary>
		Bottom, 
		
		/// <summary>
		/// The control centers itself within the parent control.
		/// </summary>
		Center, 
		
		/// <summary>
		/// The control aligns itself to the top of the parent control.
		/// </summary>
		Top, 
		
		/// <summary>
		/// The control stretches to fill the height of the parent control.
		/// </summary>
		Stretch
	};

	/// <summary>
	/// Defines vertical or horizontal orientation.
	/// </summary>
	enum class Orientation
	{
		/// <summary>
		/// Horizontal orientation.
		/// </summary>
		Horizontal = 1, 
		
		/// <summary>
		/// Vertical orientation.
		/// </summary>
		Vertical
	};

	/// <summary>
	/// Represents the thickness of a frame or border around a rectangle.
	/// </summary>
	struct Thickness
	{
	public:
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
		/// Compares two Thickness instances for equality
		/// </summary>
		/// <param name="left">- the first Thickness to compare</param>
		/// <param name="right">- the second Thickness to compare</param>
		/// <returns>True if all thickness values are equal, false otherwise</returns>
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