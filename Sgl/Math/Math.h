#pragma once

#include <array>
#include <numbers>
#include <numeric>
#include <cmath>
#include <vector>
#include <list>
#include <ranges>
#include "SDL/SDL_rect.h"
#include "SDL/SDL_render.h"
#include "../Graphic/Color.h"
#include "../Graphic/Views.h"

namespace Sgl
{
	class Math
	{
	public:
		static bool IsTriangleContainsPoint(SDL_FPoint a, SDL_FPoint b,
											SDL_FPoint c, SDL_FPoint point)
		{
			return fabsf(TriangleArea(a, b, c)
						 - TriangleArea(point, a, b)
						 + TriangleArea(point, b, c)
						 + TriangleArea(point, a, c)) < 1e-5f;
		};
		
		static constexpr float VectorProductKValue(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
		}

		static constexpr float TriangleArea(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.f;
		}

		template<size_t number>
		static std::array<float, number> ComputeSinArray()
		{
			std::array<float, number> result;
			constexpr float Step = 2 * std::numbers::pi / number;

			for(size_t i = 0; i < number; i++)
			{
				result[i] = sinf(Step * i);
			}

			return result;
		}

		template<size_t number>
		static std::array<float, number> ComputeCosArray()
		{
			std::array<float, number> result;
			constexpr float Step = 2 * std::numbers::pi / number;

			for(size_t i = 0; i < number; i++)
			{
				result[i] = cosf(Step * i);
			}

			return result;
		}

		static std::vector<int> TriangulateConvexShape(PointsView points);
		static std::vector<int> TriangulateConvexShape(PointsView points, SDL_FPoint center);
	public:
		static inline const auto sin360 = ComputeSinArray<360>();
		static inline const auto cos360 = ComputeCosArray<360>();
	};
	
}