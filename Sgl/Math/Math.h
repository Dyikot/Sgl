#pragma once

#include <array>
#include <numbers>
#include <numeric>
#include <cmath>
#include <vector>
#include <list>
#include <span>
#include <ranges>
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
		static std::vector<SDL_Vertex> CreateVertexVector(std::span<SDL_FPoint> points, Sgl::Color color);

		static constexpr float VectorProductKValue(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
		}

		static constexpr float TriangleArea(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.f;
		}		

		template<size_t Number>
		static std::array<float, Number> ComputeSin()
		{
			std::array<float, Number> result;
			constexpr float Step = 2 * std::numbers::pi / Number;

			for(size_t i = 0; i < Number; i++)
			{
				result[i] = sinf(Step * i);
			}

			return result;
		}

		template<size_t Number>
		static std::array<float, Number> ComputeCos()
		{
			std::array<float, Number> result;
			constexpr float Step = 2 * std::numbers::pi / Number;

			for(size_t i = 0; i < Number; i++)
			{
				result[i] = cosf(Step * i);
			}

			return result;
		}

		static std::array<SDL_FPoint, 90> Compute90EllipsePoints(SDL_FPoint position, int width, int height)
		{
			return ComputeEllipsePoints<90>(position, width, height);
		}

		static std::array<SDL_FPoint, 180> Compute180EllipsePoints(SDL_FPoint position, int width, int height)
		{
			return ComputeEllipsePoints<180>(position, width, height);
		}

		static std::array<SDL_FPoint, 360> Compute360EllipsePoints(SDL_FPoint position, int width, int height)
		{
			return ComputeEllipsePoints<360>(position, width, height);
		}
	private:
		template<size_t Number>
		static std::array<SDL_FPoint, Number> ComputeEllipsePoints(SDL_FPoint position, int width, int height)
		{
			const std::array<float, 360>& sin = Sin360;
			const std::array<float, 360>& cos = Cos360;
			const size_t AngleStep = 360 / Number;
			constexpr float Step = 2 * std::numbers::pi / Number;

			std::array<SDL_FPoint, Number> points;
			size_t angle = 0;

			for(size_t i = 0; i < Number; i++)
			{
				angle = i * AngleStep;
				points[i].x = position.x + width * cos[angle];
				points[i].y = position.y + height * sin[angle];
			}

			return points;
		}
	public:
		static inline Lazy<std::array<float, 360>> Sin360 = &Math::ComputeSin<360>;
		static inline Lazy<std::array<float, 360>> Cos360 = &Math::ComputeCos<360>;
	};
}