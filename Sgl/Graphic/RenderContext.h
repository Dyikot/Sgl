#pragma once

#include <string_view>
#include <array>
#include "SDL/SDL_render.h"
#include "BasicTypes.h"
#include "Color.h"
#include "Texture.h"
#include "Surface.h"

namespace Sgl
{
	class RenderContext
	{
	private:
		Renderer _renderer;
	public:
		explicit RenderContext(Renderer renderer) noexcept;
		
		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<FPoint> points, Color color);
		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<FPoint> points, Color color);
		void DrawRect(FRect rectange, Color color);
		void DrawRects(std::span<FRect> rectanges, Color color);
		void DrawFillRect(FRect rectange, Color fill);
		void DrawFillRects(std::span<FRect> rectanges, Color fill);
		void DrawTexture(Texture& texture, FRect rectangle, Color fill);
		void DrawTexture(Texture& texture, FRect rectangle, Rect clip, Color fill);
		void DrawEllipse(std::span<FPoint> ellipse, Color color);
		void DrawEllipse(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(std::span<FPoint> ellipse, Color color);
		void DrawEllipseFill(FPoint position, int width, int height, Color color);
		void DrawShape(std::span<Vertex> vertices);
		void DrawShape(std::span<Vertex> vertices, Texture& texture, Color color);
		void DrawShape(std::span<Vertex> vertices, std::span<int> order);
		void DrawShape(std::span<Vertex> vertices, std::span<int> order, Texture& texture, Color color);
		void FillBackground(Color color);
		void FillBackground(Texture& texture, Color color);
		void SetBlendMode(SDL_BlendMode mode);

		void UpdateScreen()
		{ 
			SDL_RenderPresent(_renderer);
		}
		
		void SetRenderColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}	
	};
}

