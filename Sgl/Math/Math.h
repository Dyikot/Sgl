#pragma once

#include <array>
#include <numbers>
#include <numeric>
#include <cmath>
#include <vector>
#include <list>
#include <span>
#include <ranges>
#include <algorithm>
#include <functional>
#include "SDL/SDL_rect.h"
#include "SDL/SDL_render.h"
#include "../Graphic/Color.h"
#include "../Data/Lazy.h"

namespace Sgl
{
	class Math
	{		
	public:
		static std::vector<int> TriangulateConvexShape(std::span<SDL_FPoint> points);
		static std::vector<int> TriangulateConvexShape(std::span<SDL_FPoint> points, SDL_FPoint center);
		static std::vector<SDL_FPoint> Compute90EllipsePoints(SDL_FPoint position, int width, int height);
		static std::vector<SDL_FPoint> Compute180EllipsePoints(SDL_FPoint position, int width, int height);
		static std::vector<SDL_FPoint> Compute360EllipsePoints(SDL_FPoint position, int width, int height);
		static std::vector<float> ComputeSin(size_t number);
		static std::vector<float> ComputeCos(size_t number);

		static std::vector<SDL_Vertex> ToSDLVertexVector(std::span<SDL_FPoint> points, Sgl::Color color)
		{
			std::vector<SDL_Vertex> result(points.size());
			std::ranges::transform(points, result.begin(), [color](const SDL_FPoint& point)
			{
				return SDL_Vertex{ point, color, {} };
			});
			
			return result;
		}

		static constexpr float VectorProductKValue(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
		}

		static constexpr float TriangleArea(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.f;
		}		
	private:
		static std::vector<SDL_FPoint> ComputeEllipsePoints(size_t number, SDL_FPoint position,
															int width, int height);
	public:
		static inline Lazy<std::vector<float>> Sin360 = std::bind_front(&Math::ComputeSin, 360);
		static inline Lazy<std::vector<float>> Cos360 = std::bind_front(&Math::ComputeCos, 360);
	};
}