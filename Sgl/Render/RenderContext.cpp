#include "RenderContext.h"
#include "../Tools/Log.h"
#include "../Math/Math.h"
#include <SDL/SDL_image.h>

namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* const renderer) noexcept:
		_renderer(renderer)
	{
		_instance = this;
	}

	Texture RenderContext::CreateTexture(std::string_view path)
	{
		auto texture = IMG_LoadTexture(_instance->_renderer, path.data());
		if(texture == nullptr)
		{
			PrintSDLError();
		}

		return Texture(texture);
	}

	void RenderContext::DrawPoint(SDL_FPoint point, Color color)
	{
		SetColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<SDL_FPoint> points, Color color)
	{
		SetColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(SDL_FPoint start, SDL_FPoint end, Color color)
	{
		SetColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<SDL_FPoint> points, Color color)
	{	
		SetColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectangle(const SDL_FRect& rectange, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRectangles(std::span<SDL_FRect> rectanges, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRectangle(const SDL_FRect& rectange, Color background)
	{
		SetColor(background);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRectangles(std::span<SDL_FRect> rectanges, Color background)
	{
		SetColor(background);
		SDL_RenderFillRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawImage(std::string_view path, const SDL_FRect& rectangle)
	{
		DrawTexture(Texture(CreateTexture(path)), rectangle);
	}

	void RenderContext::DrawImage(std::string_view path, const SDL_FRect& rectangle, const SDL_Rect& clip)
	{
		DrawTexture(Texture(CreateTexture(path)), rectangle, clip);
	}

	void RenderContext::DrawImage(std::string_view path, SDL_FPoint position, int width, int height)
	{
		auto texture = CreateTexture(path);
		auto points = Math::ComputeEllipsePoints(position, width, height);
		auto order = Math::Triangulate(points, position);
		std::array<SDL_Vertex, 100> vertices = {};

		Math::PointsToVertexRange(points, vertices, texture.Color);
		vertices.back() = SDL_Vertex(position, static_cast<SDL_Color>(texture.Color), SDL_FPoint());

		DrawShape(vertices, order, texture);
	}

	void RenderContext::DrawTexture(const Texture& texture, const SDL_FRect& rectangle)
	{
		SetTextureColor(texture);
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(const Texture& texture,
									const SDL_FRect& rectangle, const SDL_Rect& clip)
	{
		SetTextureColor(texture);
		SDL_RenderCopyF(_renderer, texture, &clip, &rectangle);
	}

	void RenderContext::DrawEllipse(SDL_FPoint position, int width, int height, Color color)
	{		
		auto points = Math::ComputeEllipsePoints(position, width, height);
		DrawLines(points, color);
	}

	void RenderContext::DrawEllipseFill(SDL_FPoint position, int width, int height, Color color)
	{
		auto points = Math::ComputeEllipsePoints(position, width, height);
		auto order = Math::Triangulate(points);
		std::array<SDL_Vertex, 100> vertices = {};
		Math::PointsToVertexRange(points, vertices, color);

		DrawShape(vertices, order);
	}

	void RenderContext::DrawShape(std::span<SDL_Vertex> vertices)
	{
		SDL_RenderGeometry(_renderer, nullptr,
						   vertices.data(), vertices.size(),
						   nullptr, 0);
	}

	void RenderContext::DrawShape(std::span<SDL_Vertex> vertices, const Texture& texture)
	{
		SetTextureColor(texture);
		SDL_RenderGeometry(_renderer, texture,
						   vertices.data(), vertices.size(),
						   nullptr, 0);
	}

	void RenderContext::DrawShape(std::span<SDL_Vertex> vertices, std::span<int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr,
						   vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawShape(std::span<SDL_Vertex> vertices, 
								   std::span<int> order, 
								   const Texture& texture)
	{
		SetTextureColor(texture);
		SDL_RenderGeometry(_renderer, texture, 
						   vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::SetSceneBackgroundColor(Color color)
	{
		SetColor(color);
		SDL_RenderClear(_renderer);
	}

	void RenderContext::SetSceneBackgroundTexture(const Texture& texture)
	{
		SetTextureColor(texture);
		SDL_RenderCopy(_renderer, texture, nullptr, nullptr);
	}

	void RenderContext::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}
}
