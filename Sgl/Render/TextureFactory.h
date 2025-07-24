#pragma once

#include <string_view>
#include "RenderContext.h"
#include "Texture.h"
#include "../Base/Font.h"

namespace Sgl
{
	class TextureFactory
	{
	private:
		SDL_Renderer* _renderer;
	public:
		TextureFactory();

		Texture Load(std::string_view path);
		Texture Create(TextureAccess textureAccess, Size size);
		Texture Create(TextureAccess textureAccess, Size size, RenderFragment renderFragment);
		Texture Create(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, Size size);
		Texture Create(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, Size size, RenderFragment renderFragment);
		Texture Create(const Surface& surface);
		Texture CreateWithText(const std::string& text, const Font& font);
	private:
		void RenderOnTexture(const Texture& texture, const RenderFragment& renderFragment);
	};
}