#include "TextureFactory.h"
#include "../Application.h"

namespace Sgl
{
	TextureFactory::TextureFactory():
		_renderer(App->Window.GetRenderer())
	{}

	Texture TextureFactory::Load(std::string_view path)
	{
		return Texture(IMG_LoadTexture(_renderer, path.data()));
	}

	Texture TextureFactory::Create(TextureAccess textureAccess, Size size)
	{
		return Create(SDL_PIXELFORMAT_RGBA8888, textureAccess, size);
	}

	Texture TextureFactory::Create(TextureAccess textureAccess, Size size, RenderFragment renderFragment)
	{
		auto texture = Create(textureAccess, size);
		RenderOnTexture(texture, renderFragment);
		return texture;
	}

	Texture TextureFactory::Create(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess,
									Size size)
	{
		return Texture(SDL_CreateTexture(_renderer, pixelFormat, static_cast<SDL_TextureAccess>(textureAccess), size.Width, size.Height));
	}

	Texture TextureFactory::Create(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess,
									Size size, RenderFragment renderFragment)
	{
		auto texture = Create(pixelFormat, textureAccess, size);
		RenderOnTexture(texture, renderFragment);
		return texture;
	}

	Texture TextureFactory::Create(const Surface& surface)
	{
		return Texture(SDL_CreateTextureFromSurface(_renderer, surface.GetSDLSurface()));
	}

	Texture TextureFactory::CreateWithText(const std::string& text, const Font& font)
	{
		auto sdlSurface = font.Antialiasing
			? TTF_RenderText_Blended(font, text.c_str(), font.Color)
			: TTF_RenderText_Solid(font, text.c_str(), font.Color);

		Log::PrintSDLErrorIf(sdlSurface == nullptr);

		return Create(Surface(sdlSurface));
	}

	void TextureFactory::RenderOnTexture(const Texture& texture,
										 const RenderFragment& renderFragment)
	{
		auto context = RenderContext(_renderer);
		context.SetTarget(texture);
		renderFragment(context);
		context.ResetTarget();
	}
}
