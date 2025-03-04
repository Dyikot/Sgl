#include "RenderContext.h"
#include "../Tools/Log.h"
#include "../Math/Math.h"
#include <SDL/SDL_image.h>

namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* renderer) noexcept:
		_renderer(renderer)
	{
		if(_renderer == nullptr)
		{
			Log::PrintSDLError();
		}

		_instance = this;
	}

	Texture RenderContext::CreateTexture(std::string_view path)
	{
		auto texture = IMG_LoadTexture(_instance->_renderer, path.data());
		if(texture == nullptr)
		{
			Log::PrintSDLError();
		}

		return Texture(texture);
	}

	void RenderContext::DrawPoint(SDL_FPoint point, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<SDL_FPoint> points, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(SDL_FPoint start, SDL_FPoint end, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<const SDL_FPoint> points, Color color)
	{	
		SetRenderColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectangle(const SDL_FRect& rectange, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRectangles(std::span<const SDL_FRect> rectanges, Color color)
	{
		SetRenderColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRectangle(const SDL_FRect& rectange, Color background)
	{
		SetRenderColor(background);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRectangles(std::span<const SDL_FRect> rectanges, Color background)
	{
		SetRenderColor(background);
		SDL_RenderFillRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawImage(std::string_view path, const SDL_FRect& rectangle, Color color)
	{
		DrawTexture(Texture(CreateTexture(path)), rectangle, color);
	}

	void RenderContext::DrawImage(std::string_view path, const SDL_FRect & rectangle, const SDL_Rect & clip, Color color)
	{
		DrawTexture(Texture(CreateTexture(path)), rectangle, clip, color);
	}

	void RenderContext::DrawImage(std::string_view path, SDL_FPoint position, int width, int height, Color color)
	{
		auto texture = CreateTexture(path);
		auto points = Math::ComputeEllipsePoints(position, width, height);
		auto order = Math::TriangulateConvexShape(points, position);
		std::array<SDL_Vertex, 100> vertices = {};

		Math::PointsToVertexRange(points, vertices,color);
		vertices.back() = SDL_Vertex(position, static_cast<SDL_Color>(color), SDL_FPoint());

		DrawShape(vertices, order, texture, color);
	}

	void RenderContext::DrawTexture(const Texture& texture, const SDL_FRect& rectangle, Color color)
	{
		SetTextureColor(texture, color);
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(const Texture& texture, const SDL_FRect& rectangle,
									const SDL_Rect& clip, Color color)
	{
		SetTextureColor(texture, color);
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
		auto order = Math::TriangulateConvexShape(points);
		std::array<SDL_Vertex, 100> vertices = {};
		Math::PointsToVertexRange(points, vertices, color);

		DrawShape(vertices, order);
	}

	void RenderContext::DrawShape(std::span<const SDL_Vertex> vertices)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

	void RenderContext::DrawShape(std::span<const SDL_Vertex> vertices, const Texture& texture, Color color)
	{
		SetTextureColor(texture, color);
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(), nullptr, 0);
	
	}

	void RenderContext::DrawShape(std::span<const SDL_Vertex> vertices, std::span<const int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawShape(std::span<const SDL_Vertex> vertices, std::span<const int> order, const Texture& texture, Color color)
	{
		SetTextureColor(texture, color);
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(),
						   order.data(), order.size());		
	}

	void RenderContext::FillSceneBackgroundWithColor(Color color)
	{
		SetRenderColor(color);
		SDL_RenderClear(_renderer);
	}

	void RenderContext::FillSceneBackgroundWithTexture(const Texture& texture, Color color)
	{
		SetTextureColor(texture, color);
		SDL_RenderCopy(_renderer, texture, nullptr, nullptr);
	}

	void RenderContext::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}
}
