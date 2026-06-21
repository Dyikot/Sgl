#include "Math.h"

#include <numbers>
#include <numeric>
#include <cmath>
#include <algorithm>

namespace Sgl::Math
{
	constexpr float PI = std::numbers::pi_v<float>;

	static std::vector<float> Transform(auto func, size_t count)
	{
		if(count == 0)
		{
			return {};
		}
		else if(count == 1)
		{
			return { func(0) };
		}

		const float step = 2 * PI / (count - 1);
		std::vector<float> result(count);

		for(size_t i = 0; i < count; i++)
		{
			result[i] = func(i * step);
		}

		result.back() = func(0);

		return result;
	}

	std::vector<float> SinRange(size_t count)
	{
		return Transform(std::sinf, count);
	}

	std::vector<float> CosRange(size_t count)
	{
		return Transform(std::cosf, count);
	}
}