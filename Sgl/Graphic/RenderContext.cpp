#include "RenderContext.h"
#include <SDL/SDL_image.h>
#include "../Tools/Log.h"
#include "../UI/Scene.h"

namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* renderer) noexcept:
		_renderer(renderer)
	{
		PrintSDLErrorIf(_renderer == nullptr);
	}

	TextureFactory RenderContext::GetTextureFactory() const
	{
		return TextureFactory(_renderer);
	}

	void RenderContext::DrawPoint(SDL_FPoint point, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(PointsView points, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(SDL_FPoint start, SDL_FPoint end, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(PointsView points, Color color)
	{	
		SetRenderColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectangle(SDL_FRect rectange, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRectangles(RectanglesView rectanges, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRectangle(SDL_FRect rectange, Color fill)
	{
		SetRenderColor(fill);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRectangles(RectanglesView rectanges, Color fill)
	{
		SetRenderColor(fill);
		SDL_RenderFillRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawTexture(const Texture& texture, SDL_FRect rectangle, Color fill)
	{
		SetTextureColor(texture, fill);
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(const Texture& texture, SDL_FRect rectangle,
									SDL_Rect clip, Color fill)
	{
		SetTextureColor(texture, fill);
		SDL_RenderCopyF(_renderer, texture, &clip, &rectangle);
	}

	void RenderContext::DrawEllipse(PointsView ellipse, Color color)
	{
		DrawLines(ellipse, color);
	}

	void RenderContext::DrawEllipse(SDL_FPoint position, int width, int height, Color color)
	{		
		Ellipse<90> ellipse(position, width, height);
		DrawLines(ellipse, color);
	}

	void RenderContext::DrawEllipseFill(PointsView ellipse, Color color)
	{
		VerticesCollection vertices(ellipse, color);
		auto order = Math::TriangulateConvexShape(ellipse);
		DrawShape(vertices, order);
	}

	void RenderContext::DrawEllipseFill(SDL_FPoint position, int width, int height, Color color)
	{
		Ellipse<90> ellipse(position, width, height);
		DrawEllipseFill(ellipse, color);
	}

	void RenderContext::DrawShape(VerticesView vertices)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

	void RenderContext::DrawShape(VerticesView vertices, const Texture& texture, Color color)
	{
		SetTextureColor(texture, color);
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(), nullptr, 0);	
	}

	void RenderContext::DrawShape(VerticesView vertices, std::span<const int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawShape(VerticesView vertices, std::span<const int> order, const Texture& texture, Color color)
	{
		SetTextureColor(texture, color);
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(),
						   order.data(), order.size());		
	}

	void RenderContext::SetSceneBackground(const Scene& scene)
	{
		if(scene.style->backgroundTexture)
		{
			SetTextureColor(*scene.style->backgroundTexture, scene.style->backgroundColor);
			SDL_RenderCopy(_renderer, *scene.style->backgroundTexture, nullptr, nullptr);
		}
		else
		{
			SetRenderColor(scene.style->backgroundColor);
			SDL_RenderClear(_renderer);
		}
	}

	void RenderContext::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}
}
