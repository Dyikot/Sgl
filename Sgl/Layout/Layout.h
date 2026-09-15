#pragma once

namespace Sgl
{
	//! @brief Defines how a control aligns itself horizontally in its parent control
	enum class HorizontalAlignment
	{
		Center,		// The control centers itself in the parent control.
		Left,		// The control aligns itself to the left of the parent control.
		Right,		// The control aligns itself to the right of the parent control.
		Stretch		// The control stretches to fill the width of the parent control.
	};

	//! @brief Defines how a control aligns itself vertically in its parent control
	enum class VerticalAlignment
	{
		Bottom,		// The control aligns itself to the bottom of the parent control.
		Center,		// The control centers itself within the parent control.
		Top,		// The control aligns itself to the top of the parent control.
		Stretch		// The control stretches to fill the height of the parent control.
	};

	//! @brief Defines vertical or horizontal orientation
	enum class Orientation
	{
		Horizontal,
		Vertical
	};

	//! @brief Represents the thickness of a frame or border around a rectangle
	struct Thickness
	{
	public:
		//! @brief Initializes a new instance of the Thickness structure with individual values for each side
		//! @param left The left thickness
		//! @param top The top thickness
		//! @param right The right thickness
		//! @param bottom The bottom thickness
		constexpr Thickness(int left, int top, int right, int bottom) noexcept:
			Left(left), Top(top), Right(right), Bottom(bottom)
		{}

		//! @brief Initializes a new instance of the Thickness structure with horizontal and vertical values
		//! @param horizontal The horizontal thickness (applied to left and right)
		//! @param vertical The vertical thickness (applied to top and bottom)
		constexpr Thickness(int horizontal, int vertical) noexcept:
			Left(horizontal), Top(vertical), Right(horizontal), Bottom(vertical)
		{}

		//! @brief Initializes a new instance of the Thickness structure with uniform thickness
		//! @param value The uniform thickness applied to all sides (default is 0)
		constexpr Thickness(int value = 0) noexcept:
			Thickness(value, value, value, value)
		{}

		//! @brief Thickness of the left side
		int Left;

		//! @brief Thickness of the top side
		int Top;

		//! @brief Thickness of the right side
		int Right;

		//! @brief Thickness of the bottom side
		int Bottom;

		//! @brief Creates a new thickness expanded by the specified value on all sides
		//! @param value The value to add to each side
		//! @return A new Thickness instance with increased boundaries
		constexpr Thickness Inflate(int value) const
		{
			return Thickness(Left + value, Top + value, Right + value, Bottom + value);
		}

		//! @brief Creates a new thickness decreased by the specified value on all sides
		//! @param value The value to subtract from each side
		//! @return A new Thickness instance with decreased boundaries
		constexpr Thickness Deflate(int value) const
		{
			return Thickness(Left - value, Top - value, Right - value, Bottom - value);
		}

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