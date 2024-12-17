#include "Shapes.h"
#include "../Math/Math.h"
#include <algorithm>
#include <ranges>
#include <list>
#include <numeric>

namespace Sgl
{
	void Line::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLine(Start, End, Color);
	}

	void Polyline::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLines(Points, Color);
	}

	void Polygon::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLines(Points, Color);
		renderContext.DrawLine(Points.back(), Points.front(), Color);
	}

	void FillPolygon::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawShape(Vertices, Order);
	}

	std::vector<SDL_Vertex> FillPolygon::ToVertexVector(std::span<SDL_FPoint> points,
														Sgl::Color color)
	{
		std::vector<SDL_Vertex> vertices(points.size());
		auto toVertex = [&color](const SDL_FPoint& point) { return SDL_Vertex{ point, color, {} }; };
		std::transform(points.begin(), points.end(), vertices.begin(), toVertex);
		return vertices;
	}

	std::vector<int> FillPolygon::Triangulate(std::span<SDL_FPoint> points)
	{
		constexpr size_t MinVerticesNumber = 3;
		std::list<int> order(points.size());
		std::iota(order.begin(), order.end(), 0);
		std::vector<int> resultOrder;
		resultOrder.reserve(points.size());

		auto previous = order.begin();
		auto current = std::next(previous, 1);
		auto next = std::next(current, 1);
		auto IsTriangleContainsPoint = [](SDL_FPoint a, SDL_FPoint b, 
										  SDL_FPoint c, SDL_FPoint point)
		{
			return Math::TriangleArea(a, b, c) == (Math::TriangleArea(point, a, b) +
												   Math::TriangleArea(point, b, c) +
												   Math::TriangleArea(point, a, c));
		};

		while(order.size() > MinVerticesNumber)
		{		
			bool isConvex = Math::VectorProductZValue(points[*previous], 
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

	void Rectangle::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawRectangle(Rect, Color);
	}

	void FillRectangle::OnRender(RenderContext& renderContext)
	{
		std::visit([&renderContext, this](const auto& paint)
		{
			using T = std::decay_t<decltype(paint)>;

			if constexpr(std::is_same_v<T, const Color*>)
			{
				renderContext.DrawFillRectangle(Rect, *paint);
			}
			else if constexpr(std::is_same_v<T, const Texture*>)
			{
				renderContext.DrawTexture(*paint, Rect);
			}
		}, Fill);
	}
	
	void Ellipse::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawEllipse(Position, Width, Height, Color);
	}
}
