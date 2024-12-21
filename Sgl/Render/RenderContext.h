#pragma once

#include "SDL/SDL_render.h"
#include "../Appearance/Fill.h"
#include <span>
#include <string_view>
#include <variant>
#include <array>

namespace Sgl
{
	class RenderContext
	{
	protected:
		SDL_Renderer* const _renderer;
	public:
		explicit RenderContext(SDL_Renderer* const renderer) noexcept;
		virtual ~RenderContext() noexcept { SDL_DestroyRenderer(_renderer); }

		void DrawPoint(SDL_FPoint point, Color color);
		void DrawPoints(std::span<SDL_FPoint> points, Color color);
		void DrawLine(SDL_FPoint start, SDL_FPoint end, Color color);
		void DrawLines(std::span<SDL_FPoint> points, Color color);
		void DrawRectangle(const SDL_FRect& rectange, Color color);
		void DrawRectangles(std::span<SDL_FRect> rectanges, Color color);
		void DrawFillRectangle(const SDL_FRect& rectange, Color background);
		void DrawFillRectangles(std::span<SDL_FRect> rectanges, Color background);
		void DrawTexture(const Texture& texture, const SDL_FRect& rectangle);
		void DrawTexture(const Texture& texture, const SDL_FRect& rectangle, const SDL_Rect& clip);
		void DrawEllipse(SDL_FPoint position, int width, int height, Color color);
		void DrawEllipseFill(SDL_FPoint position, int width, int height, Color color);
		void DrawShape(std::span<SDL_Vertex> vertices);
		void DrawShape(std::span<SDL_Vertex> vertices, const Texture& texture);
		void DrawShape(std::span<SDL_Vertex> vertices, std::span<int> order);
		void DrawShape(std::span<SDL_Vertex> vertices, std::span<int> order, const Texture& texture);
		void FillSceneBackground(const Fill& fill);
		void SetBlendMode(SDL_BlendMode mode);
		Texture CreateTexture(std::string_view path);

		operator SDL_Renderer* () const { return _renderer; }
	protected:
		void SetDrawColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
		}

		void SetTextureColor(const Texture& texture) const noexcept
		{
			if(texture.Color != Colors::White)
			{
				SDL_SetTextureColorMod(texture, texture.Color.R, texture.Color.G, texture.Color.B);
			}

			if(!texture.Color.IsTransparent())
			{
				SDL_SetTextureAlphaMod(texture, texture.Color.A);
			}
		}
	};
}

