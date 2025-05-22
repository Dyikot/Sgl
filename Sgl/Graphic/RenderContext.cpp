#include "RenderContext.h"
#include <SDL/SDL_image.h>
#include "../Tools/Log.h"
#include "../Math/Triangulation.h"

namespace Sgl
{
	RenderContext::RenderContext(Renderer& renderer) noexcept:
		_renderer(renderer)
	{}

	void RenderContext::DrawPoint(FPoint point, Color color)
	{
		_renderer.SetColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<const FPoint> points, Color color)
	{
		_renderer.SetColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(FPoint start, FPoint end, Color color)
	{
		_renderer.SetColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<const FPoint> points, Color color)
	{	
		_renderer.SetColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRect(FRect rectange, Color color)
	{
		_renderer.SetColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRects(std::span<const FRect> rectanges, Color color)
	{
		_renderer.SetColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRect(FRect rectange, Color fill)
	{
		_renderer.SetColor(fill);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRects(std::span<const FRect> rectanges, Color fill)
	{
		_renderer.SetColor(fill);
		SDL_RenderFillRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawTexture(Texture& texture, FRect rectangle, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(Texture& texture, FRect rectangle,
									Rect clip, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, &clip, &rectangle);
	}

	void RenderContext::DrawEllipse(const Ellipse& ellipse, Color color)
	{
		DrawLines(ellipse.GetCoordinates(), color);
	}

	void RenderContext::DrawEllipse(FPoint position, int width, int height, Color color)
	{
		auto ellipse = Ellipse(position, width, height);
		DrawLines(ellipse.GetCoordinates(), color);
	}

	void RenderContext::DrawEllipseFill(const FillEllipse& ellipse)
	{
		DrawShape(ellipse.GetVertices(), ellipse.GetVerticesOrder());
	}

	void RenderContext::DrawEllipseFill(FPoint position, int width, int height, Color color)
	{
		auto ellipse = FillEllipse(position, width, height, color);
		DrawEllipseFill(ellipse);
	}

	void RenderContext::DrawShape(std::span<const Vertex> vertices)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

	void RenderContext::DrawShape(std::span<const Vertex> vertices, Texture& texture)
	{
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(), nullptr, 0);	
	}

	void RenderContext::DrawShape(std::span<const Vertex> vertices, std::span<const int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawShape(std::span<const Vertex> vertices, std::span<const int> order,
									 Texture& texture)
	{
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(),
						   order.data(), order.size());		
	}

	void RenderContext::FillBackground(Color color)
	{
		_renderer.SetColor(color);
		SDL_RenderClear(_renderer);
	}

	void RenderContext::FillBackground(Texture& texture, Color color)
	{
		texture.SetColor(color);
		SDL_RenderCopy(_renderer, texture, nullptr, nullptr);
	}
}
