#pragma once

#include <string_view>
#include <array>
#include "SDL/SDL_render.h"
#include "../Graphic/Color.h"
#include "../Graphic/Texture.h"
#include "../Graphic/Surface.h"
#include "Ranges.h"
#include "../Math/Math.h"

namespace Sgl
{
	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit RenderContext(SDL_Renderer* renderer) noexcept;
		
		void DrawPoint(SDL_FPoint point, Color color);
		void DrawPoints(PointsView points, Color color);
		void DrawLine(SDL_FPoint start, SDL_FPoint end, Color color);
		void DrawLines(PointsView points, Color color);
		void DrawRectangle(const SDL_FRect& rectange, Color color);
		void DrawRectangles(RectanglesView rectanges, Color color);
		void DrawFillRectangle(const SDL_FRect& rectange, Color fill);
		void DrawFillRectangles(RectanglesView rectanges, Color fill);
		void DrawTexture(const Texture& texture, const SDL_FRect& rectangle, Color fill);
		void DrawTexture(const Texture& texture, const SDL_FRect& rectangle, const SDL_Rect& clip, Color fill);
		void DrawEllipse(PointsView ellipse, Color color);
		void DrawEllipse(SDL_FPoint position, int width, int height, Color color);
		void DrawEllipseFill(PointsView ellipse, Color color);
		void DrawEllipseFill(SDL_FPoint position, int width, int height, Color color);
		void DrawShape(VerticesView vertices);
		void DrawShape(VerticesView vertices, const Texture& texture, Color color);
		void DrawShape(VerticesView vertices, std::span<const int> order);
		void DrawShape(VerticesView vertices, std::span<const int> order, const Texture& texture, Color color);
		void FillSceneBackgroundWithColor(Color color);
		void FillSceneBackgroundWithTexture(const Texture& texture, Color color);
		void SetBlendMode(SDL_BlendMode mode);
		
		void SetRenderColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
		}

		void SetTextureColor(const Texture& texture, Color color) const noexcept
		{
			SDL_SetTextureColorMod(texture, color.R, color.G, color.B);
			SDL_SetTextureAlphaMod(texture, color.A);
		}

		void SetSurfaceColor(const Surface& surface, Color color) const noexcept
		{
			SDL_SetSurfaceColorMod(surface, color.R, color.G, color.B);
			SDL_SetSurfaceAlphaMod(surface, color.A);
		}

		operator SDL_Renderer* () { return _renderer; }
	};
}

