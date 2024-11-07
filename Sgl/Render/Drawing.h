#pragma once

#include "SDL/SDL_render.h"
#include "../Appearance/Brushes.h"
#include "../Appearance/Texture.h"

namespace Sgl
{
	class Drawing
	{
	protected:
		SDL_Renderer* const Renderer;
	public:
		Drawing() noexcept;
		virtual ~Drawing() = default;

		virtual void Draw() const = 0;
	protected:
		void SetRenderColor(ColorBrush brush) const noexcept
		{
			SDL_SetRenderDrawColor(Renderer, brush.R, brush.G, brush.B, brush.A);
		}

		void SetRenderColor(const TextureBrush& brush) const noexcept
		{
			SDL_SetTextureColorMod(brush.Source, brush.R, brush.G, brush.B);
			SDL_SetTextureAlphaMod(brush.Source, brush.A);
		}
	};
}

