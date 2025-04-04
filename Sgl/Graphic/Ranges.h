#pragma once

#include <array>
#include <vector>
#include "../Math/Math.h"

namespace Sgl
{
	using PointsCollection = std::vector<SDL_FPoint>;
	using RectanglesCollection = std::vector<SDL_FRect>;

	class VerticesCollection: public std::vector<SDL_Vertex>
	{
	public:
		using base = std::vector<SDL_Vertex>;

		VerticesCollection() = default;
		explicit VerticesCollection(size_t count);
		VerticesCollection(PointsView points, Color color);
		VerticesCollection(PointsView points, PointsView textureCoordinates, Color color);
	};

	template<size_t Points = 360>
	class Ellipse: public std::array<SDL_FPoint, Points>
	{
	public:
		constexpr Ellipse(SDL_FPoint position, int width, int height)
		{
			constexpr size_t MaxAngle = 360;

			if constexpr(MaxAngle % Points)
			{
				constexpr auto AngleStep = MaxAngle / Points;
				for(size_t angle = 0; SDL_FPoint& point : *this)
				{
					point.x = position.x + width * Math::Cos360[angle];
					point.y = position.y + height * Math::Sin360[angle];
					angle += AngleStep;
				}
			}
			else
			{
				constexpr float AngleStep = 2 * std::numbers::pi / Points;	
				for(float angle = 0; SDL_FPoint& point : *this)
				{
					point.x = position.x + width * cosf(angle);
					point.y = position.y + height * sinf(angle);
					angle += AngleStep;
				}
			}
		}
	};
}