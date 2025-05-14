#pragma once

#include <string_view>
#include <array>
#include "SDL/SDL_render.h"
#include "Types.h"
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
		void DrawPoints(FPointsView points, Color color);
		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(FPointsView points, Color color);
		void DrawRect(FRect rectange, Color color);
		void DrawRects(FRectsView rectanges, Color color);
		void DrawFillRect(FRect rectange, Color fill);
		void DrawFillRects(FRectsView rectanges, Color fill);
		void DrawTexture(const Texture& texture, FRect rectangle, Color fill);
		void DrawTexture(const Texture& texture, FRect rectangle, Rect clip, Color fill);
		void DrawEllipse(FPointsView ellipse, Color color);
		void DrawEllipse(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(FPointsView ellipse, Color color);
		void DrawEllipseFill(FPoint position, int width, int height, Color color);
		void DrawShape(VerticesView vertices);
		void DrawShape(VerticesView vertices, const Texture& texture, Color color);
		void DrawShape(VerticesView vertices, std::span<const int> order);
		void DrawShape(VerticesView vertices, std::span<const int> order, const Texture& texture, Color color);
		void FillBackground(Color color);
		void FillBackground(const Texture& texture, Color color);
		void SetBlendMode(SDL_BlendMode mode);

		void UpdateScreen()
		{ 
			SDL_RenderPresent(_renderer);
		}
		
		void SetRenderColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}

		void SetTextureColor(const Texture& texture, Color color) const noexcept
		{
			SDL_SetTextureColorMod(texture, color.Red, color.Green, color.Blue);
			SDL_SetTextureAlphaMod(texture, color.Alpha);
		}

		void SetSurfaceColor(const Surface& surface, Color color) const noexcept
		{
			SDL_SetSurfaceColorMod(surface, color.Red, color.Green, color.Blue);
			SDL_SetSurfaceAlphaMod(surface, color.Alpha);
		}		
	};
}

