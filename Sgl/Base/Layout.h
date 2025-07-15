#pragma once

#include "Color.h"

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

	struct Thickness
	{
	public:
		int Left;
		int Top;
		int Right;
		int Bottom;
	public:
		constexpr Thickness(int left, int top, int right, int bottom) noexcept:
			Left(left), Top(top), Right(right), Bottom(bottom)
		{}

		constexpr Thickness(int horizontal, int vertical) noexcept:
			Left(horizontal), Top(vertical), Right(horizontal), Bottom(vertical)
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