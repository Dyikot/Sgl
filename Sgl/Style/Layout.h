#pragma once

#include "../Render/Color.h"

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
		uint32_t Top;
		uint32_t Right;
		uint32_t Bottom;
		uint32_t Left;
	public:
		constexpr Thickness(uint32_t top, uint32_t right, uint32_t bottom, uint32_t left) noexcept:
			Top(top), Right(right), Bottom(bottom), Left(left)
		{}

		constexpr Thickness(uint32_t value = 0) noexcept:
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