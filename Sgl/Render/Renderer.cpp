#include "Renderer.h"
#include "../Application.h"

namespace Sgl
{
	Renderer::Renderer():
		_renderer(App->Window._renderer)
	{}

	RenderContext Renderer::OpenContext()
	{
		return RenderContext(_renderer);
	}

	Texture Renderer::LoadTexture(std::string_view path)
	{
		return Texture(IMG_LoadTexture(_renderer, path.data()));
	}

	Texture Renderer::CreateTexture(TextureAccess textureAccess, int width, int height)
	{
		return CreateTexture(SDL_PIXELFORMAT_RGBA8888, textureAccess, width, height);
	}

	Texture Renderer::CreateTexture(TextureAccess textureAccess, int width, int height, 
									RenderFragment renderFragment)
	{
		auto texture = CreateTexture(textureAccess, width, height);
		RenderOnTexture(texture, std::move(renderFragment));
		return texture;
	}

	Texture Renderer::CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess,
									int width, int height)
	{
		return Texture(SDL_CreateTexture(_renderer, pixelFormat, static_cast<SDL_TextureAccess>(textureAccess), width, height));
	}

	Texture Renderer::CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess,
									int width, int height, RenderFragment renderFragment)
	{
		auto texture = CreateTexture(pixelFormat, textureAccess, width, height);
		RenderOnTexture(texture, std::move(renderFragment));
		return texture;
	}

	Texture Renderer::CreateTexture(const Surface& surface)
	{
		return Texture(SDL_CreateTextureFromSurface(_renderer, surface));
	}

	Texture Renderer::CreateTextTexture(const std::string& text, const Font& font)
	{
		auto sdlSurface = font.Antialiasing
			? TTF_RenderText_Blended(font, text.c_str(), font.Color)
			: TTF_RenderText_Solid(font, text.c_str(), font.Color);

		Log::PrintSDLErrorIf(sdlSurface == nullptr);

		return CreateTexture(Surface(sdlSurface));
	}

	void Renderer::RenderOnTexture(Texture& texture, RenderFragment renderFragment)
	{
		SDL_SetRenderTarget(_renderer, texture);
		renderFragment(OpenContext());
		SDL_SetRenderTarget(_renderer, nullptr);
	}

	void Renderer::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}
}
