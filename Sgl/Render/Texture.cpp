#include "Texture.h"
#include "../Application.h"
#include "../Base/Log.h"

namespace Sgl
{
	static SDL_Renderer* GetRenderer()
	{
		static auto renderer = App->Window.GetRenderer();
		return renderer;
	}

	static void RenderOnTexture(const Texture& texture, const RenderFragment& renderFragment)
	{
		auto context = RenderContext(GetRenderer());
		context.SetTarget(texture);
		renderFragment(context);
		context.ResetTarget();
	}

	Texture::Texture(std::string_view path):
		_texture(IMG_LoadTexture(GetRenderer(), path.data()))
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(const Surface& surface):
		_texture(SDL_CreateTextureFromSurface(GetRenderer(), surface.GetSDLSurface()))
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(TextureAccess textureAccess, Size size):
		Texture(SDL_PIXELFORMAT_RGBA8888, textureAccess, size)
	{}

	Texture::Texture(TextureAccess textureAccess, Size size, RenderFragment renderFragment):
		Texture(textureAccess, size)
	{
		RenderOnTexture(_texture, renderFragment);
	}

	Texture::Texture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, Size size):
		_texture(SDL_CreateTexture(
			GetRenderer(),
			pixelFormat, 
			static_cast<SDL_TextureAccess>(textureAccess),
			size.Width, 
			size.Height))
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, Size size,
		RenderFragment renderFragment):
		Texture(pixelFormat, textureAccess, size)
	{
		RenderOnTexture(_texture, renderFragment);
	}

	Texture::Texture(const std::string& text, const Font& font):
		Texture(Surface(text, font))
	{}

	Texture::Texture(SDL_Texture* texture):
		_texture(texture)
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(Texture&& other) noexcept:
		_texture(std::exchange(other._texture, nullptr))
	{}

	Texture::~Texture() noexcept
	{
		if(_texture)
		{
			SDL_DestroyTexture(_texture);
		}
	}

	Size Texture::GetSize() const
	{
		int width = 0, height = 0;
		SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
		return Size(width, height);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		_texture = std::exchange(other._texture, nullptr);
		return *this;
	}
}
