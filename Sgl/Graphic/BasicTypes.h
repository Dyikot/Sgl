#pragma once

#include <span>
#include <vector>
#include <SDL/SDL_rect.h>
#include <SDL/SDL_render.h>
#include "Color.h"
#include "../Math/Math.h"

namespace Sgl
{
	using FPoint = SDL_FPoint;
	using Point = SDL_Point;
	using FRect = SDL_FRect;
	using Rect = SDL_Rect;
	using Vertex = SDL_Vertex;
	using Renderer = SDL_Renderer*;

	class VerticesCollection: public std::vector<Vertex>
	{
	public:
		using base = std::vector<Vertex>;

		VerticesCollection() = default;
		explicit VerticesCollection(size_t count);
		VerticesCollection(std::span<FPoint> points, Color color);
		VerticesCollection(std::span<FPoint> points, std::span<FPoint> textureCoordinates, Color color);
	};

	class Shapes
	{
	public:
		static std::vector<FPoint> CreateEllipse(FPoint position, int width, int height, size_t pointsNumber = 360);
	};
}