#include "Math.h"

namespace Sgl
{
	std::vector<float> Math::SinRange(size_t size) noexcept
	{
		const double step = 2 * std::numbers::pi / static_cast<double>(size);
		std::vector<float> result(size);

		for(size_t i = 0; i < size; i++)
		{
			result[i] = sinf(step * i);
		}

		return result;
	}

	std::vector<float> Math::CosRange(size_t size) noexcept
	{
		const double step = 2 * std::numbers::pi / static_cast<double>(size);
		std::vector<float> result(size);

		for(size_t i = 0; i < size; i++)
		{
			result[i] = cosf(step * i);
		}

		return result;
	}

	std::vector<int> Math::TriangulateEllipse(size_t pointsNumber)
	{
		std::vector<int> order(pointsNumber * 3);
		constexpr int center = 0;
		int step = center;

		for(size_t i = 0; i < order.size(); i += 3, step++)
		{
			order[i] = center;
			order[i + 1] = step + 1;
			order[i + 2] = step + 2;
		}

		order[order.size() - 1] = 1;

		return order;
	}

	bool Math::IsPointInRect(FPoint point, FRect rect) noexcept
	{
		return point.x >= rect.x &&
			   point.x <= rect.x + rect.w &&
			   point.y >= rect.y &&
			   point.y <= rect.y + rect.h;
	}
}