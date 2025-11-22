#include "Texture.h"
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

namespace Sgl
{
	Texture::Texture(std::nullptr_t):
		_texture(nullptr)
	{}

	Texture::Texture(SDL_Renderer* renderer, std::string_view path):
		_texture(IMG_LoadTexture(renderer, path.data()))
	{
		if(_texture == nullptr)
		{
			SDL_Log("Unable to create a texture: %s", SDL_GetError());
		}
	}

	Texture::Texture(SDL_Renderer* renderer, Size size, 
					 TextureAccess access, 
					 SDL_PixelFormat format):
		_texture(SDL_CreateTexture(renderer,
				format,
				SDL_TextureAccess(access),
				size.Width,
				size.Height))
	{
		if(_texture == nullptr)
		{
			SDL_Log("Unable to create a texture: %s", SDL_GetError());
		}
	}

	Texture::Texture(SDL_Renderer* renderer, FontQuality fontQuality, TTF_Font* font, 
					 std::string_view text, Color foreground, Color background)
	{
		SDL_Surface* surface = nullptr;

		switch(fontQuality)
		{
			case FontQuality::Blended:
				surface = TTF_RenderText_Blended(font, text.data(), text.length(), foreground);
				break;
			case FontQuality::Solid:
				surface = TTF_RenderText_Solid(font, text.data(), text.length(), foreground);
				break;
			case FontQuality::Shaded:
				surface = TTF_RenderText_Shaded(font, text.data(), text.length(), foreground, background);
				break;
			case FontQuality::LCD:
				surface = TTF_RenderText_LCD(font, text.data(), text.length(), foreground, background);
				break;
		}

		_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);

		if(_texture == nullptr)
		{
			SDL_Log("Unable to create a texture: %s", SDL_GetError());
		}
	}

	Texture::Texture(SDL_Renderer* renderer, FontQuality fontQuality, TTF_Font* font, 
					 std::string_view text, int wrapWidth, Color foreground, Color background)
	{
		SDL_Surface* surface = nullptr;

		switch(fontQuality)
		{
			case FontQuality::Blended:
				surface = TTF_RenderText_Blended_Wrapped(font, text.data(), text.length(), foreground, wrapWidth);
				break;
			case FontQuality::Solid:
				surface = TTF_RenderText_Solid_Wrapped(font, text.data(), text.length(), foreground, wrapWidth);
				break;
			case FontQuality::Shaded:
				surface = TTF_RenderText_Shaded_Wrapped(font, text.data(), text.length(), foreground, background, wrapWidth);
				break;
			case FontQuality::LCD:
				surface = TTF_RenderText_LCD_Wrapped(font, text.data(), text.length(), foreground, background, wrapWidth);
				break;
		}

		_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);

		if(_texture == nullptr)
		{
			SDL_Log("Unable to create a texture: %s", SDL_GetError());
		}
	}

	Texture::~Texture()
	{
		if(_texture != nullptr)
		{
			SDL_DestroyTexture(_texture);
		}
	}

	void Texture::SetColor(Color value)
	{
		SDL_SetTextureColorMod(_texture, value.Red, value.Green, value.Blue);
		SDL_SetTextureAlphaMod(_texture, value.Alpha);
	}

	Color Texture::GetColor() const
	{
		Color color = Colors::Transparent;
		SDL_GetTextureColorMod(_texture, &color.Red, &color.Green, &color.Blue);
		SDL_GetTextureAlphaMod(_texture, &color.Alpha);
		return color;
	}

	void Texture::SetBlendMode(SDL_BlendMode value)
	{
		SDL_SetTextureBlendMode(_texture, value);
	}

	SDL_BlendMode Texture::GetBlendMode() const
	{
		SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
		SDL_GetTextureBlendMode(_texture, &blendMode);
		return blendMode;
	}

	void Texture::SetScaleMode(SDL_ScaleMode value)
	{
		SDL_SetTextureScaleMode(_texture, value);
	}

	SDL_ScaleMode Texture::GetScaleMode() const
	{
		SDL_ScaleMode scaleMode = SDL_SCALEMODE_LINEAR;
		SDL_GetTextureScaleMode(_texture, &scaleMode);
		return scaleMode;
	}

	Size Texture::GetSize() const
	{
		return Size(_texture->w, _texture->h);
	}

	TextureAccess Texture::GetAccess() const
	{
		auto id = SDL_GetTextureProperties(_texture);

		if(id == 0)
		{
			SDL_Log("Unable to get a texture property: %s", SDL_GetError());
		}

		return TextureAccess(SDL_GetNumberProperty(id, SDL_PROP_TEXTURE_ACCESS_NUMBER, 0));
	}

	SDL_PixelFormat Texture::GetFormat() const
	{
		return _texture->format;
	}

	SDL_Texture* Texture::GetSDLTexture() const noexcept
	{
		return _texture;
	}

	TextureLockContext Texture::Lock(std::optional<Rect> rect)
	{
		return TextureLockContext(*this, rect);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		_texture = std::exchange(other._texture, _texture);
		return *this;
	}

	TextureLockContext::TextureLockContext(Texture& texture, std::optional<Rect> rect):
		_texture(texture),
		_pixels(),
		_pitch()
	{
		const Rect* lockRect = rect.has_value() ? &rect.value() : nullptr;
		_height = lockRect ? lockRect->h : texture.GetSize().Height;

		if(SDL_LockTexture(_texture.GetSDLTexture(), lockRect, &_pixels, &_pitch) < 0)
		{
			SDL_Log("Unable to lock a texture: %s", SDL_GetError());
		}
	}

	TextureLockContext::~TextureLockContext()
	{
		if(_pixels)
		{
			SDL_UnlockTexture(_texture.GetSDLTexture());
		}
	}

	bool TextureLockContext::HasLock() const noexcept
	{
		return _pixels != nullptr;
	}

	void* TextureLockContext::GetPixels() const noexcept
	{
		return _pixels;
	}

	int TextureLockContext::GetPitch() const noexcept
	{
		return _pitch;
	}
}
