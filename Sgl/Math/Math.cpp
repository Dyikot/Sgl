#include "Math.h"

namespace Sgl
{
    std::vector<int> Math::TriangulateConvexShape(PointsView points)
    {
		constexpr size_t MinVerticesNumber = 3;
		std::list<int> order(points.size());
		std::iota(order.begin(), order.end(), 0);
		std::vector<int> resultOrder;
		resultOrder.reserve(points.size());

		auto previous = order.begin();
		auto current = std::next(previous, 1);
		auto next = std::next(current, 1);		

		while(order.size() > MinVerticesNumber)
		{
			bool isConvex = Math::VectorProductKValue(points[*previous],
													  points[*current],
													  points[*next]) < 0;
			bool isTriangleContainsPoint = false;

			if(isConvex)
			{
				for(size_t i = 0; !isTriangleContainsPoint && i < *previous; i++)
				{
					isTriangleContainsPoint = IsTriangleContainsPoint(
						points[*previous], points[*current], points[*next], points[i]);
				}

				if(auto afterNext = std::next(next, 1); afterNext != order.end())
				{
					for(int i = *afterNext; !isTriangleContainsPoint && i < points.size(); i++)
					{
						isTriangleContainsPoint = IsTriangleContainsPoint(
							points[*previous], points[*current], points[*next], points[i]);
					}
				}
			}

			if(isTriangleContainsPoint || !isConvex)
			{
				previous = current;
			}

			if(!isTriangleContainsPoint)
			{
				resultOrder.push_back(*previous);
				resultOrder.push_back(*current);
				resultOrder.push_back(*next);
				order.remove(*current);
			}

			current = next;
			next++;

			if(next == order.end())
			{
				next = order.begin();
			}
		}

		return resultOrder;
    }

	std::vector<int> Math::TriangulateConvexShape(PointsView points, SDL_FPoint center)
	{
		std::vector<int> resultOrder;
		resultOrder.reserve((points.size()) * 3);
		size_t centerIndex = points.size();

		for(size_t i = 0; i < points.size() - 1; i++)
		{
			resultOrder.push_back(centerIndex);
			resultOrder.push_back(i);
			resultOrder.push_back(i + 1);
		}

		resultOrder.push_back(centerIndex);
		resultOrder.push_back(points.size() - 1);
		resultOrder.push_back(0);

		return resultOrder;
	}
}
