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
	class Scene;

	class RenderContext
	{
	private:
		Renderer _renderer;
	public:
		explicit RenderContext(Renderer renderer) noexcept;
		
		void DrawPoint(Point point, Color color);
		void DrawPoints(PointsView points, Color color);
		void DrawLine(Point start, Point end, Color color);
		void DrawLines(PointsView points, Color color);
		void DrawRectangle(Rect rectange, Color color);
		void DrawRectangles(RectanglesView rectanges, Color color);
		void DrawFillRectangle(Rect rectange, Color fill);
		void DrawFillRectangles(RectanglesView rectanges, Color fill);
		void DrawTexture(const Texture& texture, Rect rectangle, Color fill);
		void DrawTexture(const Texture& texture, Rect rectangle, iRect clip, Color fill);
		void DrawEllipse(PointsView ellipse, Color color);
		void DrawEllipse(Point position, int width, int height, Color color);
		void DrawEllipseFill(PointsView ellipse, Color color);
		void DrawEllipseFill(Point position, int width, int height, Color color);
		void DrawShape(VerticesView vertices);
		void DrawShape(VerticesView vertices, const Texture& texture, Color color);
		void DrawShape(VerticesView vertices, std::span<const int> order);
		void DrawShape(VerticesView vertices, std::span<const int> order, const Texture& texture, Color color);
		void SetSceneBackground(const Scene& scene);
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

