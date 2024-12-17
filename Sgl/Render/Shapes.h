#pragma once

#include "RenderContext.h"
#include "IVisual.h"
#include <vector>
#include <stdexcept>

namespace Sgl
{
	class Line: public IVisual
	{
	public:
		SDL_FPoint Start;
		SDL_FPoint End;
		Color Color;
	public:
		Line(float x1, float y1, float x2, float y2, Sgl::Color color):
			Start{ x1, y1 }, End{ x2, y2 }, Color(color)
		{}
		Line(SDL_FPoint start, SDL_FPoint end, Sgl::Color color):
			Start(start), End(end), Color(color)
		{}

		void OnRender(RenderContext& renderContext) override;
	};

	class Polyline: public IVisual
	{
	public:
		std::vector<SDL_FPoint> Points;
		Color Color;
	public:
		Polyline(std::span<SDL_FPoint> points, Sgl::Color color):
			Points(points.begin(), points.end()), Color(color)
		{}
		Polyline(std::vector<SDL_FPoint>&& points, Sgl::Color color):
			Points(std::move(points)), Color(color)
		{}

		void OnRender(RenderContext& renderContext) override;
	};

	class Polygon: public IVisual
	{
	public:
		std::vector<SDL_FPoint> Points;
		Color Color;
	public:
		Polygon(std::span<SDL_FPoint> points, Sgl::Color color):
			Points(points.begin(), points.end()), Color(color)
		{}
		Polygon(std::vector<SDL_FPoint>&& points, Sgl::Color color):
			Points(std::move(points)), Color(color)
		{}

		void OnRender(RenderContext& renderContext) override;
	};

	class FillPolygon: public IVisual
	{
	public:
		std::vector<SDL_Vertex> Vertices;
		std::vector<int> Order;
		Color Color;
	public:
		FillPolygon(std::span<SDL_FPoint> points, Sgl::Color color):
			Vertices(ToVertexVector(points, color)), 
			Order(Triangulate(points)),
			Color(color)
		{}

		void OnRender(RenderContext& renderContext) override;
	public:
		static std::vector<SDL_Vertex>
			ToVertexVector(std::span<SDL_FPoint> points, Sgl::Color color);
		static std::vector<int> Triangulate(std::span<SDL_FPoint> points);
	};

	class Rectangle: public IVisual
	{
	public:
		SDL_FRect Rect;
		Color Color;
	public:
		Rectangle(float x, float y, float width, float height, Sgl::Color color):
			Rect{ x, y, width, height }, Color(color)
		{}
		Rectangle(SDL_FPoint position, float width, float height, Sgl::Color color):
			Rect{ position.x, position.y, width, height }, Color(color)
		{}
		Rectangle(SDL_FRect rect, Sgl::Color color):
			Rect(rect), Color(color)
		{}

		void OnRender(RenderContext& renderContext) override;
	};

	class FillRectangle: public IVisual
	{
	public:
		SDL_FRect Rect;
		Fill Fill;
	public:
		FillRectangle(float x, float y, float width, float height, Sgl::Fill paint):
			Rect{ x, y, width, height }, Fill(paint)
		{}
		FillRectangle(SDL_FPoint position, float width, float height, Sgl::Fill paint):
			Rect{ position.x, position.y, width, height }, Fill(paint)
		{}
		FillRectangle(const SDL_FRect& rect, Sgl::Fill paint):
			Rect(rect), Fill(paint)
		{}

		void OnRender(RenderContext& renderContext) override;
	};

	class Ellipse: public IVisual
	{
	public:
		SDL_FPoint Position;
		int Width;
		int Height;
		Color Color;
	public:
		Ellipse(SDL_FPoint position, int width, int height, Sgl::Color color):
			Position(position), Width(width), Height(height), Color(color)
		{}

		void OnRender(RenderContext& renderContext) override;
	};
}