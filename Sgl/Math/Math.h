#pragma once

#include <vector>
#include <numbers>
#include <numeric>
#include <cmath>

namespace Sgl
{
	class Math
	{
	public:
		template<size_t Number>
		static std::vector<float> SinRange() noexcept
		{
			constexpr double step = 2 * std::numbers::pi / Number;
			auto result = std::vector<float>(Number);

			for(size_t i = 0; i < Number; i++)
			{
				result[i] = sinf(step * i);
			}

			return result;
		}

		template<size_t Number>
		static std::vector<float> CosRange() noexcept
		{
			constexpr double step = 2 * std::numbers::pi / Number;
			auto result = std::vector<float>(Number);

			for(size_t i = 0; i < Number; i++)
			{
				result[i] = cosf(step * i);
			}

			return result;
		}

		static std::vector<int> TriangulateEllipse(size_t pointsNumber);
	};	
}