#include "RenderContext.h"
#include <SDL/SDL_image.h>
#include "../Tools/Log.h"
#include "../Math/Triangulation.h"

namespace Sgl
{
	RenderContext::RenderContext(Renderer renderer) noexcept:
		_renderer(renderer)
	{
		PrintSDLErrorIf(_renderer == nullptr);
	}

	void RenderContext::DrawPoint(FPoint point, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<FPoint> points, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(FPoint start, FPoint end, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<FPoint> points, Color color)
	{	
		SetRenderColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRect(FRect rectange, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRects(std::span<FRect> rectanges, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRect(FRect rectange, Color fill)
	{
		SetRenderColor(fill);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRects(std::span<FRect> rectanges, Color fill)
	{
		SetRenderColor(fill);
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

	void RenderContext::DrawEllipse(std::span<FPoint> ellipse, Color color)
	{
		DrawLines(ellipse, color);
	}

	void RenderContext::DrawEllipse(FPoint position, int width, int height, Color color)
	{		
		auto ellipsePoints = Shapes::CreateEllipse(position, width, height);
		DrawLines(ellipsePoints, color);
	}

	void RenderContext::DrawEllipseFill(std::span<FPoint> ellipse, Color color)
	{
		auto vertices = VerticesCollection( ellipse, color);
		auto order = Math::TriangulateConvexShape(ellipse);
		DrawShape(vertices, order);
	}

	void RenderContext::DrawEllipseFill(FPoint position, int width, int height, Color color)
	{
		auto ellipsePoints = Shapes::CreateEllipse(position, width, height);
		DrawEllipseFill(ellipsePoints, color);
	}

	void RenderContext::DrawShape(std::span<Vertex> vertices)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

	void RenderContext::DrawShape(std::span<Vertex> vertices, Texture& texture, Color color)
	{
		texture.SetColor(color);
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(), nullptr, 0);	
	}

	void RenderContext::DrawShape(std::span<Vertex> vertices, std::span<int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawShape(std::span<Vertex> vertices, std::span<int> order,
								  Texture& texture, Color color)
	{
		texture.SetColor(color);
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(),
						   order.data(), order.size());		
	}

	void RenderContext::FillBackground(Color color)
	{
		SetRenderColor(color);
		SDL_RenderClear(_renderer);
	}

	void RenderContext::FillBackground(Texture& texture, Color color)
	{
		texture.SetColor(color);
		SDL_RenderCopy(_renderer, texture, nullptr, nullptr);
	}

	void RenderContext::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}
}
