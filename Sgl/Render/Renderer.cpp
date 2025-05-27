#include "Renderer.h"
#include "../Application.h"

namespace Sgl
{
	Renderer::Renderer():
		_renderer(Application::Get()->Window._renderer)
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

	Texture Renderer::CreateTexture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess,
									int width, int height)
	{
		return Texture(SDL_CreateTexture(_renderer, pixelFormat, static_cast<SDL_TextureAccess>(textureAccess), width, height));
	}

	void Renderer::FillTexture(Texture& texture, Action<RenderContext> action)
	{
		SDL_SetRenderTarget(_renderer, texture);
		action(OpenContext());
		SDL_SetRenderTarget(_renderer, nullptr);
	}

	void Renderer::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}
}
