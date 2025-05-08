#pragma once

#include <array>
#include <numbers>
#include <numeric>
#include <cmath>

namespace Sgl::Math
{
	template<size_t number>
	inline std::array<float, number> SinRange() noexcept
	{
		constexpr double step = 2 * std::numbers::pi / number;
		std::array<float, number> result;

		for(size_t i = 0; i < number; i++)
		{
			result[i] = sinf(step * i);
		}

		return result;
	}

	template<size_t number>
	inline std::array<float, number> CosRange() noexcept
	{
		constexpr double step = 2 * std::numbers::pi / number;
		std::array<float, number> result;

		for(size_t i = 0; i < number; i++)
		{
			result[i] = cosf(step * i);
		}

		return result;
	}
}