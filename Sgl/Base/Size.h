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

		bool operator==(const FSize&) const = default;
	};

	constexpr FSize operator+(const FSize& left, const FSize& right) noexcept
	{
		return FSize(left.Width + right.Width, left.Height + right.Height);
	}

	constexpr FSize operator-(const FSize& left, const FSize& right) noexcept
	{
		return FSize(left.Width - right.Width, left.Height - right.Height);
	}
}