#pragma once

#include "SDL/SDL_render.h"
#include "../Appearance/Texture.h"
#include "GeometryFigures.h"

namespace Sgl
{
	class RenderContext
	{
	public:
		SDL_Renderer* const Renderer;
	public:
		RenderContext(SDL_Renderer* const renderer) noexcept;
		virtual ~RenderContext() = default;

		void Draw(const Line& line) const;
		void Draw(const Lines& lines) const;
		void Draw(const Rectangle& rectange) const;
		void Draw(const Rectangles& rectanges) const;
		void Draw(const FillRectangle& rectange) const;
		void Draw(const FillRectangles& rectanges) const;
		void Draw(const TextureRectangle& rectange) const;
		void Draw(const Figure& figure) const;
		void Draw(const Ellipse& ellipse) const;
		void DrawSceneBackground(const Brush& background) const;
	protected:
		void SetRenderColor(ColorBrush brush) const noexcept
		{
			SDL_SetRenderDrawColor(Renderer, brush.R, brush.G, brush.B, brush.A);
		}

		void SetRenderColor(const TextureBrush& brush) const noexcept
		{
			if(brush != Colors::White)
			{
				SDL_SetTextureColorMod(brush.RawTexture, brush.R, brush.G, brush.B);
			}

			if(brush.IsOpaque())
			{
				SDL_SetTextureAlphaMod(brush.RawTexture, brush.A);
			}
		}

		void FillBackground(const ColorBrush& brush) const noexcept;
		void FillBackground(const TextureBrush& brush) const noexcept;
	};
}

