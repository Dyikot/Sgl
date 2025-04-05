#include "Ranges.h"
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
			push_back(SDL_Vertex{ point, color, {} });
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
			push_back(SDL_Vertex{ points[i], color, textureCoordinates[i]});
		}
	}

}

