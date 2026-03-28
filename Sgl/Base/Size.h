#pragma once

namespace Sgl
{
	// <summary>
	/// Represents a 2D size with unsigned integer dimensions.
	/// </summary>
	struct Size
	{
		uint32_t Width;
		uint32_t Height;
	};

	/// <summary>
	/// Represents a 2D size with floating-point dimensions.
	/// </summary>
	struct FSize
	{
		float Width;
		float Height;

		auto operator<=>(const FSize&) const = default;
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