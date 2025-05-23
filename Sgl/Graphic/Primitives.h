#pragma once

#include <SDL/SDL_rect.h>
#include <SDL/SDL_render.h>
#include <vector>
#include "Texture.h"
#include "Color.h"

namespace Sgl
{
	using FPoint = SDL_FPoint;
	using Point = SDL_Point;
	using FRect = SDL_FRect;
	using Rect = SDL_Rect;
	using Vertex = SDL_Vertex;

	class Ellipse
	{
	private:
		std::vector<FPoint> _coordinates;
	public:
		Ellipse(FPoint position, int width, int height);

		const std::vector<FPoint>& GetCoordinates() const;
	private:
		static size_t SelectPointsNumber(int pixels);
	};

	class FillEllipse
	{
	public:
		Texture* Texture = nullptr;
	private:
		std::vector<Vertex> _vertices;
		std::vector<int> _order;
	public:
		FillEllipse(FPoint position, int width, int height, Color color);
		FillEllipse(FPoint position, int width, int height, Sgl::Texture& texture, Color color);

		const std::vector<Vertex>& GetVertices() const;
		const std::vector<int>& GetVerticesOrder() const;
	private:
		static size_t SelectPointsNumber(int pixels);
		std::vector<int> TriangulateEllipse(size_t pointsNumber);
	};
}