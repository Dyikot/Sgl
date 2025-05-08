#pragma once

#include <vector>
#include "../Math/Math.h"
#include "Views.h"
#include "Color.h"

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

	template<size_t points = 360>
	class Ellipse: public std::array<SDL_FPoint, points>
	{
	private:
		static inline auto sinRange = Math::SinRange<360>();
		static inline auto cosRange = Math::CosRange<360>();
	public:
		constexpr Ellipse(SDL_FPoint position, int width, int height)
		{
			constexpr size_t maxAngle = 360;

			if constexpr(maxAngle % points)
			{
				constexpr auto angleStep = maxAngle / points;
				size_t angle = 0;

				for(SDL_FPoint& point : *this)
				{
					point.x = position.x + width * cosRange[angle];
					point.y = position.y + height * sinRange[angle];
					angle += angleStep;
				}
			}
			else
			{
				constexpr float AngleStep = 2 * std::numbers::pi / points;	

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