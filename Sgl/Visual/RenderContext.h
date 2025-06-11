#pragma once

#include <string_view>
#include <array>
#include <span>
#include <SDL/SDL_render.h>
#include "Surface.h"
#include "Primitives.h"
#include "Texture.h"
#include "../Data/Delegate.h"

namespace Sgl
{
	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit RenderContext(SDL_Renderer* renderer) noexcept;
		
		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<const FPoint> points, Color color);
		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<const FPoint> points, Color color);
		void DrawRect(FRect rectange, Color color);
		void DrawRects(std::span<const FRect> rectanges, Color color);
		void DrawFillRect(FRect rectange, Color fill);
		void DrawFillRects(std::span<const FRect> rectanges, Color fill);
		void DrawTexture(const Texture& texture, FRect rectangle);
		void DrawTexture(const Texture& texture, FRect rectangle, Color fill);
		void DrawTexture(Texture& texture, FRect rectangle, Rect clip);
		void DrawTexture(Texture& texture, FRect rectangle, Rect clip, Color fill);
		void DrawCircle(FPoint position, int diameter, Color color);
		void DrawCircleFill(FPoint position, int diameter, Color color);
		void DrawCircleFill(FPoint position, int diameter, Texture& texture, Color color);
		void DrawEllipse(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(FPoint position, int width, int height, const Texture& texture, Color color);
		void DrawGeometry(std::span<const Vertex> vertices);
		void DrawGeometry(std::span<const Vertex> vertices, const Texture& texture);
		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order);
		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order, const Texture& texture);

		void FillBackground(Color color);
		void FillBackground(Texture& texture, Color color);
	private:
		void SetColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}
	};

	using RenderFragment = Action<RenderContext>;
}

