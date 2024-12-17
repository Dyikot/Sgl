#pragma once
#include <SDL/SDL_rect.h>
#include <array>
#include <numbers>
#include <cmath>

namespace Sgl
{
	class Math
	{
	public:
		static constexpr float VectorProductZValue(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
		}

		static constexpr float TriangleArea(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c)
		{
			return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2;
		}

		static std::array<SDL_FPoint, 100>
			CalculateEllipsePoints(SDL_FPoint position, int width, int height)
		{
			constexpr size_t PointsNumber = 100;
			static auto sin = CalculateSin<PointsNumber>();
			static auto cos = CalculateCos<PointsNumber>();
			constexpr float Step = 2 * std::numbers::pi / (PointsNumber - 1);
			std::array<SDL_FPoint, PointsNumber> points;

			for(int i = 0; i < PointsNumber; i++)
			{
				points[i].x = position.x + width * cos[i];
				points[i].y = position.y + height * sin[i];
			}

			return points;
		}

		template <size_t PointsNumber>
		static std::array<SDL_FPoint, PointsNumber>
			CalculateEllipsePoints(SDL_FPoint position, int width, int height)
		{
			constexpr float Step = 2 * std::numbers::pi / (PointsNumber - 1);
			std::array<SDL_FPoint, PointsNumber> points;

			for(int i = 0; i < PointsNumber; i++)
			{
				points[i].x = position.x + width * std::cosf(Step * i);
				points[i].y = position.y + height * std::sinf(Step * i);
			}

			return points;
		}
	protected:
		template<size_t Number>
		static std::array<float, Number> CalculateSin()
		{
			std::array<float, Number> sin;
			float Step = 2 * std::numbers::pi / (Number - 1);
			for(int i = 0; i < Number; i++)
			{
				sin[i] = std::sinf(Step * i);
			}

			return sin;
		}

		template<size_t Number>
		static std::array<float, Number> CalculateCos()
		{
			std::array<float, Number> cos;
			float Step = 2 * std::numbers::pi / (Number - 1);
			for(int i = 0; i < Number; i++)
			{
				cos[i] = std::cosf(Step * i);
			}

			return cos;
		}
	};
}