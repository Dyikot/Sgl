#include "Math.h"

namespace Sgl
{
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
}