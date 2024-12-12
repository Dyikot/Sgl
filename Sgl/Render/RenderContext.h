#pragma once

#include <span>
#include <string_view>
#include "SDL/SDL_render.h"
#include "../Appearance/Texture.h"
#include <variant>

namespace Sgl
{
	using Point = SDL_FPoint;
	using Rectangle = SDL_FRect;
	using Vertex = SDL_Vertex;
	using Paint = std::variant<const Color*, const Texture*>;

	class RenderContext
	{
	protected:
		SDL_Renderer* const _renderer;
	public:
		RenderContext(SDL_Renderer* const renderer) noexcept;
		virtual ~RenderContext() noexcept { SDL_DestroyRenderer(_renderer); }

		void DrawLine(Point start, Point end, Color color);
		void DrawLines(std::span<Point> points, Color color);
		void DrawRectangle(const Rectangle& rectange, Color color);
		void DrawRectanges(std::span<Rectangle> rectanges, Color color);
		void DrawFillRectangle(const Rectangle& rectange, Color background);
		void DrawFillRectanges(std::span<Rectangle> rectanges, Color background);
		void DrawTexture(Texture& texture, const Rectangle& rectangle);
		void DrawTexture(Texture& texture, const Rectangle& rectangle, const Rectangle& clip);
		void DrawFigure(std::span<Vertex> vertices, std::span<int> order, Texture& texture);
		void DrawEllipse(Point position, int width, int height, Color color);
		void DrawSceneBackground(const Paint& background);
		void SetBlendMode(SDL_BlendMode blendMode);
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
	private:
		void CalculateEllipseVertices(std::span<Point> vertices, Point position, 
									  int width, int height);
	};
}

