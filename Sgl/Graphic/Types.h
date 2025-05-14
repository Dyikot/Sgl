#pragma once
#include <SDL/SDL_rect.h>
#include <SDL/SDL_render.h>
#include <span>
#include <vector>
#include "../Math/Math.h"
#include "Color.h"

namespace Sgl
{
	using FPoint = SDL_FPoint;
	using Point = SDL_Point;
	using FRect = SDL_FRect;
	using Rect = SDL_Rect;
	using Vertex = SDL_Vertex;
	using Renderer = SDL_Renderer*;

	using FPointsView = std::span<const FPoint>;
	using FRectsView = std::span<const FRect>;
	using VerticesView = std::span<const Vertex>;

	using FPointsCollection = std::vector<FPoint>;
	using FRectsCollection = std::vector<FRect>;

	class VerticesCollection: public std::vector<Vertex>
	{
	public:
		using base = std::vector<Vertex>;

		VerticesCollection() = default;
		explicit VerticesCollection(size_t count);
		VerticesCollection(FPointsView points, Color color);
		VerticesCollection(FPointsView points, FPointsView textureCoordinates, Color color);
	};

	class Shapes
	{
	public:
		static FPointsCollection CreateEllipse(FPoint position, int width, int height, size_t pointsNumber = 360);
	};
}