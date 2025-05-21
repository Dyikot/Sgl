#include "Renderer.h"
#include "../Tools/Log.h"

namespace Sgl
{
	Renderer::Renderer(SDL_Window* window):
		_renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))
	{
		PrintSDLErrorIf(_renderer == nullptr);
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(_renderer);
	}
}
