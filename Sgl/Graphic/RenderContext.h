#pragma once

#include <string_view>
#include <array>
#include "SDL/SDL_render.h"
#include "Surface.h"
#include "Renderer.h"
#include "Primitives.h"
#include <span>

namespace Sgl
{
	class RenderContext
	{
	private:
		Renderer& _renderer;
	public:
		explicit RenderContext(Renderer& renderer) noexcept;
		
		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<const FPoint> points, Color color);
		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<const FPoint> points, Color color);
		void DrawRect(FRect rectange, Color color);
		void DrawRects(std::span<const FRect> rectanges, Color color);
		void DrawFillRect(FRect rectange, Color fill);
		void DrawFillRects(std::span<const FRect> rectanges, Color fill);
		void DrawTexture(Texture& texture, FRect rectangle, Color fill);
		void DrawTexture(Texture& texture, FRect rectangle, Rect clip, Color fill);
		void DrawEllipse(const Ellipse& ellipse, Color color);
		void DrawEllipse(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(const FillEllipse& ellipse);
		void DrawEllipseFill(FPoint position, int width, int height, Color color);
		void DrawShape(std::span<const Vertex> vertices);
		void DrawShape(std::span<const Vertex> vertices, Texture& texture);
		void DrawShape(std::span<const Vertex> vertices, std::span<const int> order);
		void DrawShape(std::span<const Vertex> vertices, std::span<const int> order, Texture& texture);
		void FillBackground(Color color);
		void FillBackground(Texture& texture, Color color);
	};
}

