#pragma once

#include <string_view>
#include "RenderContext.h"
#include "Texture.h"
#include "Font.h"

namespace Sgl
{
	using RenderFragment = Action<RenderContext>;

	template<typename T>
	using RenderFragmentFor = RenderFragment(*)(const T&);

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
		Texture CreateTexture(const Surface& surface);
		Texture CreateTextTexture(const std::string& text, const Font& font);
		void RenderOnTexture(Texture& texture, RenderFragment renderFragment);

		void SetBlendMode(SDL_BlendMode mode);

		void UpdateScreen() 
		{
			SDL_RenderPresent(_renderer);
		}
	};
}