#include "RenderContext.h"
#include "../Tools/Log.h"
#include <SDL/SDL_image.h>
#include <numbers>
#include <array>
#include <variant>
#include <format>
#include <iostream>
namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* const renderer) noexcept:
		_renderer(renderer)
	{}

	void RenderContext::DrawLine(Point start, Point end, Color color)
	{
		SetDrawColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<Point> points, Color color)
	{	
		SetDrawColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectangle(const Rectangle& rectange, Color color)
	{
		SetDrawColor(color);
		SDL_RenderDrawRectF(_renderer, &rectange);
	}

	void RenderContext::DrawRectanges(std::span<Rectangle> rectanges, Color color)
	{
		SetDrawColor(color);
		SDL_RenderDrawRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawFillRectangle(const Rectangle& rectange, Color background)
	{
		SetDrawColor(background);
		SDL_RenderFillRectF(_renderer, &rectange);		
	}

	void RenderContext::DrawFillRectanges(std::span<Rectangle> rectanges, Color background)
	{
		SetDrawColor(background);
		SDL_RenderFillRectsF(_renderer, rectanges.data(), rectanges.size());
	}

	void RenderContext::DrawTexture(const Texture& texture, const Rectangle& rectangle)
	{
		SetTextureColor(texture);
		SDL_RenderCopyF(_renderer, texture, nullptr, &rectangle);
	}

	void RenderContext::DrawTexture(const Texture& texture,
									const Rectangle& rectangle,
									const Rectangle& clip)
	{
		SDL_Rect iclip = { clip.x, clip.y, clip.w, clip.h };
		SetTextureColor(texture);
		SDL_RenderCopyF(_renderer, texture, &iclip, &rectangle);
	}

	void RenderContext::DrawFigure(std::span<Vertex> vertices, 
								   std::span<int> order, 
								   const Texture& texture)
	{
		SetTextureColor(texture);
		SDL_RenderGeometry(_renderer, texture, 
						   vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawEllipse(Point position, int width, int height, Color color)
	{
		constexpr size_t PointNumber = 100;
		std::array<Point, PointNumber> points;
		CalculateEllipseVertices(points, position, width, height);

		SetDrawColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), PointNumber);
	}

	void RenderContext::PaintSceneBackground(const Paint& background)
	{
		std::visit([this](const auto& paint)
		{
			using T = std::decay_t<decltype(paint)>;

			if constexpr(std::is_same_v<T, const Color*>)
			{
				SetDrawColor(*paint);
				SDL_RenderClear(_renderer);
			}
			else if constexpr(std::is_same_v<T, const Texture*>)
			{
				SetTextureColor(*paint);
				SDL_RenderCopy(_renderer, *paint, nullptr, nullptr);
			}
		}, background);
	}

	void RenderContext::PaintBackground(const Rectangle& reactangle, const Paint& background)
	{
		std::visit([this, &reactangle](const auto& paint)
		{
			using T = std::decay_t<decltype(paint)>;

			if constexpr(std::is_same_v<T, const Color*>)
			{
				DrawFillRectangle(reactangle, *paint);
			}
			else if constexpr(std::is_same_v<T, const Texture*>)
			{
				DrawTexture(*paint, reactangle);
			}
		}, background);
	}

	void RenderContext::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}

	Texture RenderContext::CreateTexture(std::string_view path)
	{
		auto texture = IMG_LoadTexture(_renderer, path.data());
		if(texture == nullptr)
		{
			PrintSDLError();
		}

		return Texture(texture);
	}

	void RenderContext::CalculateEllipseVertices(std::span<Point> vertices, Point position,
												 int width, int height)
	{
		float step = 2 * std::numbers::pi / (vertices.size() - 1);

		for(int i = 0; i < vertices.size(); i++)
		{
			vertices[i].x = position.x + width * std::cosf(step * i);
			vertices[i].y = position.y + height * std::sinf(step * i);
		}
	}
}
