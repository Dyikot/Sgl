#include "Collections.h"
#include <cassert>

namespace Sgl
{
	VerticesCollection::VerticesCollection(size_t count):
		base(count)
	{}

	VerticesCollection::VerticesCollection(PointsView points, Color color)
	{
		reserve(points.size());

		for(auto& point : points)
		{
			push_back(SDL_Vertex 
			{
				.position = point,
				.color = color,
				.tex_coord = SDL_FPoint() 
			});
		}
	}

	VerticesCollection::VerticesCollection(PointsView points, 
										   PointsView textureCoordinates,
										   Color color)
	{
		assert(points.size() == textureCoordinates.size());

		reserve(points.size());

		for(size_t i = 0; i < points.size(); i++)
		{
			push_back(SDL_Vertex 
			{
				.position = points[i],
				.color = color,
				.tex_coord = textureCoordinates[i]
			});
		}
	}
}

