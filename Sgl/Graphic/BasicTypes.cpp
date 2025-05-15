#include "BasicTypes.h"
#include <cassert>

namespace Sgl
{
	VerticesCollection::VerticesCollection(size_t count):
		base(count)
	{}

	VerticesCollection::VerticesCollection(std::span<FPoint> points, Color color)
	{
		reserve(points.size());

		for(auto& point : points)
		{
			push_back(Vertex
			{
				.position = point,
				.color = color,
				.tex_coord = SDL_FPoint()
			});
		}
	}

	VerticesCollection::VerticesCollection(std::span<FPoint> points,
								   std::span<FPoint> textureCoordinates,
								   Color color)
	{
		assert(points.size() == textureCoordinates.size());

		reserve(points.size());

		for(size_t i = 0; i < points.size(); i++)
		{
			push_back(Vertex
			{
				.position = points[i],
				.color = color,
				.tex_coord = textureCoordinates[i]
			});
		}
	}

	static inline auto _sinRange = Math::SinRange<360>();
	static inline auto _cosRange = Math::CosRange<360>();

	std::vector<FPoint> Shapes::CreateEllipse(FPoint position, int width, int height, size_t pointsNumber)
	{
		constexpr size_t maxAngle = 360;
		auto points = std::vector<FPoint>(pointsNumber);

		if (maxAngle % pointsNumber)
		{
			const auto angleStep = maxAngle / pointsNumber;
			size_t angle = 0;

			for(auto& point : points)
			{
				point.x = position.x + width * _cosRange[angle];
				point.y = position.y + height * _sinRange[angle];
				angle += angleStep;
			}
		}
		else
		{
			const float angleStep = 2 * std::numbers::pi / pointsNumber;
			float angle = 0;

			for(auto& point : points)
			{
				point.x = position.x + width * cosf(angle);
				point.y = position.y + height * sinf(angle);
				angle += angleStep;
			}
		}

		return points;
	}
}