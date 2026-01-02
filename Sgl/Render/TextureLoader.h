#pragma once

#include "Texture.h"

struct SDL_Renderer;

namespace Sgl
{
	class TextureLoader
	{
	private:
		SDL_Renderer*& _renderer;
	public:
		explicit TextureLoader(SDL_Renderer*& renderer): _renderer(renderer) {}

		Texture Load(std::string_view filePath)
		{
			return Texture(_renderer, filePath);
		}
	};
}