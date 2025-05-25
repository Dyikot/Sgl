#pragma once

#include <SDL/SDL_render.h>
#include "Color.h"

namespace Sgl
{
	class Renderer
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit Renderer(SDL_Window* window);
		~Renderer();

		void SetColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}

		void SetBlendMode(SDL_BlendMode mode)
		{
			SDL_SetRenderDrawBlendMode(_renderer, mode);
		}

		void UpdateScreen()
		{
			SDL_RenderPresent(_renderer);
		}

		operator SDL_Renderer* () const noexcept
		{
			return _renderer;
		}
	};
}