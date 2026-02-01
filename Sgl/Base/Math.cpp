#include "Math.h"

#include <numbers>
#include <numeric>
#include <cmath>

namespace Sgl::Math
{
	std::vector<float> SinRange(size_t count) noexcept
	{
		if(count == 0)
		{
			return {};
		}
		else if(count == 1)
		{
			return { 0.f };
		}

		const float step = 2.f * std::numbers::pi_v<float> / static_cast<float>(count - 1);
		std::vector<float> result(count);

		for(size_t i = 0; i < count; i++)
		{
			result[i] = sinf(i * step);
		}

		return result;
	}

	std::vector<float> CosRange(size_t count) noexcept
	{
		if(count == 0)
		{
			return {};
		}
		else if(count == 1)
		{
			return { 1.f };
		}

		const float step = 2.f * std::numbers::pi_v<float> / static_cast<float>(count - 1);
		std::vector<float> result(count);

		for(size_t i = 0; i < count; i++)
		{
			result[i] = cosf(i * step);
		}

		return result;
	}

	std::vector<int> TriangulateEllipse(size_t verticesNumber)
	{
		std::vector<int> order(verticesNumber * 3);
		constexpr int center = 0;
		int step = center;

		for(size_t i = 0; i < order.size(); i += 3)
		{
			order[i] = center;
			order[i + 1] = step + 1;
			order[i + 2] = step + 2;
			++step;
		}

		order.back() = 1;

		return order;
	}
}