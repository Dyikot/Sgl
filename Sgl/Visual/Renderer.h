#pragma once

#include <string_view>
#include "RenderContext.h"
#include "Texture.h"

namespace Sgl
{
	class Renderer
	{
	private:
		SDL_Renderer* _renderer;
	public:
		Renderer();

		RenderContext OpenContext();

		Texture LoadTexture(std::string_view path);
		Texture CreateTexture(TextureAccess textureAccess, int width, int height);
		Texture CreateTexture(TextureAccess textureAccess, int width, int height, RenderFragment renderFragment);
		Texture CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, int width, int height);
		Texture CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, int width, int height, RenderFragment renderFragment);
		void RenderOnTexture(Texture& texture, RenderFragment renderFragment);

		void SetBlendMode(SDL_BlendMode mode);

		void UpdateScreen() 
		{
			SDL_RenderPresent(_renderer);
		}
	};
}