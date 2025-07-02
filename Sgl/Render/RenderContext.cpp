#include "RenderContext.h"
#include <SDL/SDL_image.h>
#include "../Tools/Log.h"
#include "../Tools/Time/Stopwatch.h"
#include "../Math/Math.h"

namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* renderer) noexcept:
		_renderer(renderer)
	{}

	void RenderContext::DrawPoint(FPoint point, Color color)
	{
		SetColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<const FPoint> points, Color color)
	{
		SetColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(FPoint start, FPoint end, Color color)
	{
		SetColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<const FPoint> points, Color color)
	{	
		SetColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectange(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectF(_renderer, &rect);
	}

	void RenderContext::DrawRectangles(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectsF(_renderer, rects.data(), rects.size());
	}

	void RenderContext::DrawFillRectangle(FRect rect, Color fill)
	{
		SetColor(fill);
		SDL_RenderFillRectF(_renderer, &rect);		
	}

	void RenderContext::DrawFillRectangles(std::span<const FRect> rects, Color fill)
	{
		SetColor(fill);
		SDL_RenderFillRectsF(_renderer, rects.data(), rects.size());
	}

	void RenderContext::DrawTexture(const Texture& texture)
	{
		SDL_RenderCopyF(_renderer, texture, nullptr, nullptr);
	}

	void RenderContext::DrawTexture(const Texture& texture, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, nullptr, nullptr);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target)
	{
		SDL_RenderCopyF(_renderer, texture, nullptr, &target);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, nullptr, &target);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target, Rect source)
	{
		SDL_RenderCopyF(_renderer, texture, &source, &target);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target,
									Rect source, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, &source, &target);
	}

	void RenderContext::DrawTextureWithRotation(const Texture& texture, double angle, FPoint center)
	{
		SDL_RenderCopyExF(_renderer, texture, nullptr, nullptr, angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureWithRotation(const Texture& texture, Color fill, 
												double angle, FPoint center)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, nullptr, nullptr, angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureWithRotation(const Texture& texture, FRect target,
												double angle, FPoint center)
	{
		SDL_RenderCopyExF(_renderer, texture, nullptr, &target, angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureWithRotation(const Texture& texture, FRect target, 
												Color fill, double angle, FPoint center)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, nullptr, &target, angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureWithRotation(const Texture& texture, FRect target, Rect source,
												double angle, FPoint center)
	{
		SDL_RenderCopyExF(_renderer, texture, &source, &target, angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureWithRotation(const Texture& texture, FRect target, Rect source,
												Color fill, double angle, FPoint center)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, &source, &target, angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureWithFlip(const Texture& texture, TextureFlip flip)
	{
		SDL_RenderCopyExF(_renderer, texture, nullptr, nullptr, 0, nullptr, 
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithFlip(const Texture& texture, Color fill, TextureFlip flip)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, nullptr, nullptr, 0, nullptr,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithFlip(const Texture& texture, FRect target, TextureFlip flip)
	{
		SDL_RenderCopyExF(_renderer, texture, nullptr, &target, 0, nullptr,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithFlip(const Texture& texture, FRect target, Color fill,
											TextureFlip flip)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, nullptr, &target, 0, nullptr,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithFlip(const Texture& texture, FRect target, Rect source,
											TextureFlip flip)
	{
		SDL_RenderCopyExF(_renderer, texture, &source, &target, 0, nullptr,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithFlip(const Texture& texture, FRect target, Rect source,
											Color fill, TextureFlip flip)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, &source, &target, 0, nullptr,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithRotationAndFlip(
		const Texture& texture, double angle, FPoint center, TextureFlip flip)
	{
		SDL_RenderCopyExF(_renderer, texture, nullptr, nullptr, angle, &center, 
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithRotationAndFlip(
		const Texture& texture, Color fill, double angle, 
		FPoint center, TextureFlip flip)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, nullptr, nullptr, angle, &center,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithRotationAndFlip(
		const Texture& texture, FRect target, double angle, 
		FPoint center, TextureFlip flip)
	{
		SDL_RenderCopyExF(_renderer, texture, nullptr, &target, angle, &center,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithRotationAndFlip(
		const Texture& texture, FRect target, Color fill, 
		double angle, FPoint center, TextureFlip flip)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, nullptr, &target, angle, &center,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithRotationAndFlip(
		const Texture& texture, FRect target, Rect source,
		double angle, FPoint center, TextureFlip flip)
	{
		SDL_RenderCopyExF(_renderer, texture, &source, &target, angle, &center,
						  static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureWithRotationAndFlip(
		const Texture& texture, FRect target, Rect source, 
		Color fill, double angle, FPoint center, TextureFlip flip)
	{
		texture.SetColor(fill);
		SDL_RenderCopyExF(_renderer, texture, &source, &target, angle, &center,
						  static_cast<SDL_RendererFlip>(flip));
	}

	static constexpr size_t MaxPointsNumber = 180;
	static const auto SinRange = Math::SinRange<MaxPointsNumber>();
	static const auto CosRange = Math::CosRange<MaxPointsNumber>();

	void RenderContext::DrawCircle(FPoint position, int diameter, Color color)
	{
		int radius = diameter / 2;
		int x = radius;
		int y = 0;
		int p = 0;

		auto center = FPoint(position.x + radius, position.y + radius);
		auto points = std::vector<FPoint>();
		points.reserve(9 * radius);

		while(x >= y)
		{
			points.emplace_back(center.x + x, center.y + y);
			points.emplace_back(center.x + y, center.y + x);
			points.emplace_back(center.x - y, center.y + x);
			points.emplace_back(center.x - x, center.y + y);
			points.emplace_back(center.x - x, center.y - y);
			points.emplace_back(center.x - y, center.y - x);
			points.emplace_back(center.x + y, center.y - x);
			points.emplace_back(center.x + x, center.y - y);

			if(p <= 0)
			{
				y += 1;
				p += 2 * y + 1;
			}
			else
			{
				x -= 1;
				p -= 2 * x + 1;
			}
		}

		DrawPoints(points, color);
	}

	void RenderContext::DrawCircleFill(FPoint position, int diameter, Color color)
	{
		DrawEllipseFill(position, diameter, diameter, color);
	}

	void RenderContext::DrawCircleFill(FPoint position, int diameter, const Texture& texture, Color color)
	{
		DrawEllipseFill(position, diameter, diameter, texture, color);
	}

	void RenderContext::DrawEllipse(FPoint position, int width, int height, Color color)
	{
		auto points = std::array<FPoint, MaxPointsNumber + 1>();
		auto radiusX = width / 2.0f;
		auto radiusY = height / 2.0f;
		auto center = FPoint(position.x + radiusX, position.y + radiusY);

		for(size_t i = 0; i < points.size() - 1; i++)
		{
			points[i].x = center.x + radiusX * CosRange[i];
			points[i].y = center.y + radiusY * SinRange[i];
		}

		points[MaxPointsNumber] = points[0];

		DrawLines(points, color);
	}

	void CalculateFillEllipse(std::span<Vertex> vertices, size_t angleStep, 
							  FPoint position, int width, int height, Color color)
	{
		auto angle = 0ull;
		auto radiusX = width / 2.0f;
		auto radiusY = height / 2.0f;
		auto center = FPoint(position.x + radiusX, position.y + radiusY);

		auto& first = vertices[0];
		first.position.x = center.x + radiusX * CosRange[0];
		first.position.y = center.y + radiusY * SinRange[0];
		first.color = color;

		for(size_t i = 1; i < vertices.size(); i++)
		{
			angle = (i - 1) * angleStep;
			vertices[i].position.x = center.x + radiusX * CosRange[angle];
			vertices[i].position.y = center.y + radiusY * SinRange[angle];
			vertices[i].color = color;
		}
	}

	void RenderContext::DrawEllipseFill(FPoint position, int width, int height, Color color)
	{
		constexpr auto pointNumber = MaxPointsNumber / 2;
		auto vertices = std::array<Vertex, pointNumber + 1>();

		CalculateFillEllipse(vertices, MaxPointsNumber / pointNumber, 
							 position, width, height, color);
		auto order = Math::TriangulateEllipse(pointNumber);
		DrawGeometry(vertices, order);
	}

	void RenderContext::DrawEllipseFill(FPoint position, int width, int height, const Texture& texture, Color color)
	{
		constexpr auto pointNumber = MaxPointsNumber / 2;
		auto vertices = std::array<Vertex, pointNumber + 1>();

		CalculateFillEllipse(vertices, MaxPointsNumber / pointNumber, 
							 position, width, height, color);
		auto order = Math::TriangulateEllipse(pointNumber);
		DrawGeometry(vertices, order, texture);
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, const Texture& texture)
	{
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(), nullptr, 0);	
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order,
								  const Texture& texture)
	{
		SDL_RenderGeometry(_renderer, texture, vertices.data(), vertices.size(),
						   order.data(), order.size());		
	}

	void RenderContext::FillBackground(Color color)
	{
		SetColor(color);
		SDL_RenderClear(_renderer);
	}
}
