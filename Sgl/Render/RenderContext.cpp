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

	void RenderContext::DrawRect(FRect rectange, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRects(std::span<const FRect> rectanges, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRect(FRect rectange, Color fill)
	{
		SetColor(fill);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRects(std::span<const FRect> rectanges, Color fill)
	{
		SetColor(fill);
		SDL_RenderFillRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect rectangle)
	{
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect rectangle, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(Texture& texture, FRect rectangle, Rect clip)
	{
		SDL_RenderCopyF(_renderer, texture, &clip, &rectangle);
	}

	void RenderContext::DrawTexture(Texture& texture, FRect rectangle,
									Rect clip, Color fill)
	{
		texture.SetColor(fill);
		SDL_RenderCopyF(_renderer, texture, &clip, &rectangle);
	}

	void RenderContext::DrawCircle(FPoint position, int diameter, Color color)
	{
		int radius = diameter / 2;
		int x = radius;
		int y = 0;
		int err = 0;

		auto center = FPoint(position.x + radius, position.y + radius);
		auto points = std::vector<FPoint>(9 * radius);

		for(int i = 0; x >= y;)
		{
			points[i++] = FPoint(center.x + x, center.y + y);
			points[i++] = FPoint(center.x + y, center.y + x);
			points[i++] = FPoint(center.x - y, center.y + x);
			points[i++] = FPoint(center.x - x, center.y + y);
			points[i++] = FPoint(center.x - x, center.y - y);
			points[i++] = FPoint(center.x - y, center.y - x);
			points[i++] = FPoint(center.x + y, center.y - x);
			points[i++] = FPoint(center.x + x, center.y - y);

			if(err <= 0)
			{
				y += 1;
				err += 2 * y + 1;
			}
			else
			{
				x -= 1;
				err -= 2 * x + 1;
			}
		}

		DrawPoints(points, color);
	}

	constexpr size_t MaxPointsNumber = 180;
	const auto SinRange = Math::SinRange<MaxPointsNumber>();
	const auto CosRange = Math::CosRange<MaxPointsNumber>();

	void RenderContext::DrawEllipse(FPoint position, int width, int height, Color color)
	{
		std::array<FPoint, MaxPointsNumber + 1> coordinates;
		float radiusX = width / 2.0f;
		float radiusY = height / 2.0f;
		FPoint center = { position.x + radiusX, position.y + radiusY };

		for(size_t i = 0; i < coordinates.size() - 1; i++)
		{
			coordinates[i].x = center.x + radiusX * CosRange[i];
			coordinates[i].y = center.y + radiusY * SinRange[i];
		}

		coordinates[MaxPointsNumber] = coordinates[0];

		DrawLines(coordinates, color);
	}

	void CalculateFillEllipse(std::span<Vertex> vertices, size_t angleStep, 
							  FPoint position, int width, int height, Color color)
	{
		size_t angle = 0;
		float radiusX = width / 2.0f;
		float radiusY = height / 2.0f;
		FPoint center = { position.x + radiusX, position.y + radiusY };

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
		constexpr size_t pointNumber = MaxPointsNumber / 2;
		std::array<Vertex, pointNumber + 1> vertices;

		CalculateFillEllipse(vertices, MaxPointsNumber / pointNumber, 
							 position, width, height, color);
		auto order = Math::TriangulateEllipse(pointNumber);
		DrawGeometry(vertices, order);
	}

	void RenderContext::DrawEllipseFill(FPoint position, int width, int height, const Texture& texture, Color color)
	{
		constexpr size_t pointNumber = MaxPointsNumber / 2;
		std::array<Vertex, pointNumber + 1> vertices;

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

	void RenderContext::FillBackground(Texture& texture, Color color)
	{
		texture.SetColor(color);
		SDL_RenderCopy(_renderer, texture, nullptr, nullptr);
	}
}
