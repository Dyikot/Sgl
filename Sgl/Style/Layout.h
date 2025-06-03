#pragma once

#include "../Visual/Color.h"

namespace Sgl
{
	enum class HorizontalAlignment
	{
		Center, Left, Right, Stretch
	};

	enum class VerticalAlignment
	{
		Bottom, Center, Top, Stretch
	};

	enum class Visibility
	{
		Visible, Hidden, Collapsed
	};

	struct Thickness
	{
	public:
		int Top;
		int Right;
		int Bottom;
		int Left;
	public:
		constexpr Thickness(int top, int right, int bottom, int left) noexcept:
			Top(top), Right(right), Bottom(bottom), Left(left)
		{}

		constexpr Thickness(int value = 0) noexcept:
			Thickness(value, value, value, value)
		{}

		friend constexpr bool operator==(const Thickness& left, const Thickness& right)
		{
			return left.Top == right.Top &&
				   left.Right == right.Right &&
				   left.Bottom == right.Bottom &&
				   left.Left == right.Left;
		}
	};
}