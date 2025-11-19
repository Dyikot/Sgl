#include "Math.h"

namespace Sgl
{
	std::vector<float> Math::SinRange(size_t count) noexcept
	{
		const float step = 2.f * std::numbers::pi / static_cast<float>(count - 1);
		std::vector<float> result(count);

		float angle = 0.f;
		for(size_t i = 0; i < count; i++)
		{
			result[i] = sinf(angle);
			angle += step;
		}

		return result;
	}

	std::vector<float> Math::CosRange(size_t count) noexcept
	{
		const float step = 2.f * std::numbers::pi / static_cast<float>(count - 1);
		std::vector<float> result(count);

		float angle = 0.f;
		for(size_t i = 0; i < count; i++)
		{
			result[i] = cosf(angle);
			angle += step;
		}

		return result;
	}

	std::vector<int> Math::TriangulateEllipse(size_t count)
	{
		std::vector<int> order(count * 3);
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