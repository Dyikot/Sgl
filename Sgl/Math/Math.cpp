#include "Math.h"

namespace Sgl
{
	inline float IsTriangleContainsPoint(SDL_FPoint a, SDL_FPoint b,
										 SDL_FPoint c, SDL_FPoint point)
	{
		return fabsf(Math::TriangleArea(a, b, c) 
					 - Math::TriangleArea(point, a, b) 
					 + Math::TriangleArea(point, b, c) 
					 + Math::TriangleArea(point, a, c)) < 1e-5f;
	};

    std::vector<int> Math::TriangulateConvexShape(std::span<SDL_FPoint> points)
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
				for(int i = 0; !isTriangleContainsPoint && i < *previous; i++)
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

	std::vector<int> Math::TriangulateConvexShape(std::span<SDL_FPoint> points, SDL_FPoint center)
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

	std::vector<SDL_FPoint> Math::Compute90EllipsePoints(SDL_FPoint position, int width, int height)
	{
		return ComputeEllipsePoints(90, position, width, height);
	}

	std::vector<SDL_FPoint> Math::Compute180EllipsePoints(SDL_FPoint position, int width, int height)
	{
		return ComputeEllipsePoints(180, position, width, height);
	}

	std::vector<SDL_FPoint> Math::Compute360EllipsePoints(SDL_FPoint position, int width, int height)
	{
		return ComputeEllipsePoints(360, position, width, height);
	}

	std::vector<float> Math::ComputeSin(size_t number)
	{
		std::vector<float> result(number);
		const float Step = 2 * std::numbers::pi / number;
		for(int i = 0; i < number; i++)
		{
			result[i] = sinf(Step * i);
		}

		return result;
	}

	std::vector<float> Math::ComputeCos(size_t number)
	{
		std::vector<float> result(number);
		const float Step = 2 * std::numbers::pi / number;
		for(int i = 0; i < number; i++)
		{
			result[i] = cosf(Step * i);
		}

		return result;
	}

	std::vector<SDL_FPoint> Math::ComputeEllipsePoints(size_t number, SDL_FPoint position,
													   int width, int height)
	{
		const std::vector<float>& sin = Sin360;
		const std::vector<float>& cos = Cos360;
		const size_t AngleStep = 360 / number;
		const float Step = 2 * std::numbers::pi / number;
		size_t angle = 0;
		std::vector<SDL_FPoint> points;
		points.resize(number);

		for(int i = 0; i < number; i++)
		{
			angle = i * AngleStep;
			points[i].x = position.x + width * cos[angle];
			points[i].y = position.y + height * sin[angle];
		}

		return points;
	}
}
