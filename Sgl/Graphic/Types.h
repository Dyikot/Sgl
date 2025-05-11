#pragma once
#include <SDL/SDL_rect.h>
#include <SDL/SDL_render.h>
#include <span>
#include <vector>
#include "../Math/Math.h"
#include "Color.h"

namespace Sgl
{
	using Point = SDL_FPoint;
	using iPoint = SDL_Point;
	using Rect = SDL_FRect;
	using iRect = SDL_Rect;
	using Vertex = SDL_Vertex;
	using Renderer = SDL_Renderer*;

	using PointsView = std::span<const Point>;
	using RectanglesView = std::span<const Rect>;
	using VerticesView = std::span<const Vertex>;

	using PointsCollection = std::vector<Point>;
	using RectanglesCollection = std::vector<Rect>;

	class VerticesCollection: public std::vector<Vertex>
	{
	public:
		using base = std::vector<Vertex>;

		VerticesCollection() = default;
		explicit VerticesCollection(size_t count);
		VerticesCollection(PointsView points, Color color);
		VerticesCollection(PointsView points, PointsView textureCoordinates, Color color);
	};

	class Shapes
	{
	public:
		static PointsCollection CreateEllipse(Point position, int width, int height, size_t pointsNumber = 360);
	};
}