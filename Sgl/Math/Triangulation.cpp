#include "Triangulation.h"
#include <cmath>
#include <list>
#include <numeric>

namespace Sgl::Math
{
	inline float TriangleArea(FPoint a, FPoint b, FPoint c)
	{
		return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.f;
	}

	inline bool DoesTriangleContainsPoint(FPoint a, FPoint b, FPoint c, FPoint point)
	{
		auto abc = TriangleArea(a, b, c);
		auto pab = TriangleArea(point, a, b);
		auto pbc = TriangleArea(point, b, c);
		auto pac = TriangleArea(point, a, c);

		return fabsf(abc - pab + pbc + pac) < 1e-5f;
	}

	inline float VectorProductKValue(FPoint a, FPoint b, FPoint c)
	{
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
	}

	std::vector<int> TriangulateConvexShape(std::span<FPoint> points)
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
			bool isConvex = VectorProductKValue(points[*previous], points[*current], points[*next]) < 0;
			bool isTriangleContainsPoint = false;

			if(isConvex)
			{
				for(size_t i = 0; !isTriangleContainsPoint && i < *previous; i++)
				{
					isTriangleContainsPoint = DoesTriangleContainsPoint(
						points[*previous], points[*current], points[*next], points[i]);
				}

				if(auto afterNext = std::next(next, 1); afterNext != order.end())
				{
					for(int i = *afterNext; !isTriangleContainsPoint && i < points.size(); i++)
					{
						isTriangleContainsPoint = DoesTriangleContainsPoint(
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

	std::vector<int> TriangulateConvexShape(std::span<FPoint> points, FPoint center)
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
