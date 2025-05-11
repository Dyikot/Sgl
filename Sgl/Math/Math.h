#pragma once

#include <vector>
#include <numbers>
#include <numeric>
#include <cmath>

namespace Sgl::Math
{
	template<size_t Number>
	inline std::vector<float> SinRange() noexcept
	{
		constexpr double step = 2 * std::numbers::pi / Number;
		std::vector<float> result = { Number };

		for(size_t i = 0; i < Number; i++)
		{
			result[i] = sinf(step * i);
		}

		return result;
	}

	template<size_t Number>
	inline std::vector<float> CosRange() noexcept
	{
		constexpr double step = 2 * std::numbers::pi / Number;
		std::vector<float> result = { Number };

		for(size_t i = 0; i < Number; i++)
		{
			result[i] = cosf(step * i);
		}

		return result;
	}
}