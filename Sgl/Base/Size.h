#pragma once

namespace Sgl
{
	//! @brief Represents a 2D size with unsigned integer dimensions
	struct Size
	{
		uint32_t Width;
		uint32_t Height;
	};

	//! @brief Represents a 2D size with floating-point dimensions
	struct FSize
	{
		float Width;
		float Height;

		constexpr bool operator==(const FSize&) const noexcept = default;

		constexpr FSize& operator+=(FSize other) noexcept
		{
			Width += other.Width;
			Height += other.Height;
			return *this;
		}

		constexpr FSize& operator-=(FSize other) noexcept
		{
			Width -= other.Width;
			Height -= other.Height;
			return *this;
		}
	};

	constexpr FSize operator+(FSize left, FSize right) noexcept
	{
		return FSize(left.Width + right.Width, left.Height + right.Height);
	}

	constexpr FSize operator-(FSize left, FSize right) noexcept
	{
		return FSize(left.Width - right.Width, left.Height - right.Height);
	}
}