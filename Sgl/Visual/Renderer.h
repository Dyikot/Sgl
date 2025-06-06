#pragma once

#include <string_view>
#include "RenderContext.h"
#include "Texture.h"
#include "../Data/Delegate.h"

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
		Texture CreateTexture(TextureAccess textureAccess, int width, int height, Action<RenderContext> fill);
		Texture CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, int width, int height);
		Texture CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, int width, int height, Action<RenderContext> fill);
		void FillTexture(Texture& texture, Action<RenderContext> fill);

		void SetBlendMode(SDL_BlendMode mode);		

		void UpdateScreen()
		{
			SDL_RenderPresent(_renderer);
		}
	};
}