#include "Texture.h"
#include "../Application.h"
#include "../Base/Log.h"

namespace Sgl
{
	struct TextureDeleter
	{
		void operator()(SDL_Texture* texture) const
		{
			SDL_DestroyTexture(texture);
		}
	};

	static SDL_Renderer* GetRenderer()
	{
		static auto renderer = App->GetMainWindow()->GetSDLRenderer();
		return renderer;
	}

	Texture::Texture(std::nullptr_t)
	{}

	Texture::Texture(std::string_view path):
		_texture(IMG_LoadTexture(GetRenderer(), path.data()), TextureDeleter())
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(Size size, TextureAccess access, SDL_PixelFormatEnum format):
		_texture(
			SDL_CreateTexture(GetRenderer(),
							  format,
							  SDL_TextureAccess(access),
							  size.Width,
							  size.Height),
			TextureDeleter())
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(FontRenderType renderType, 
					 TTF_Font* font, 
					 std::string_view text, 
					 Color foreground, 
					 Color background)
	{
		SDL_Surface* surface = nullptr;

		switch(renderType)
		{
			case Sgl::FontRenderType::Blended:
				surface = TTF_RenderUTF8_Blended(font, text.data(), foreground);
				break;
			case Sgl::FontRenderType::Solid:
				surface = TTF_RenderUTF8_Solid(font, text.data(), foreground);
				break;
			case Sgl::FontRenderType::Shaded:
				surface = TTF_RenderUTF8_Shaded(font, text.data(), foreground, background);
				break;
			case Sgl::FontRenderType::LCD:
				surface = TTF_RenderUTF8_LCD(font, text.data(), foreground, background);
				break;
		}

		_texture = { SDL_CreateTextureFromSurface(GetRenderer(), surface), TextureDeleter() };
		SDL_FreeSurface(surface);

		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	Texture::Texture(FontRenderType renderType, 
					 TTF_Font * font, 
					 std::string_view text,
					 unsigned int wrapLength, 
					 Color foreground, 
					 Color background)
	{
		SDL_Surface* surface = nullptr;

		switch(renderType)
		{
			case Sgl::FontRenderType::Blended:
				surface = TTF_RenderUTF8_Blended_Wrapped(font, text.data(), foreground, wrapLength);
				break;
			case Sgl::FontRenderType::Solid:
				surface = TTF_RenderUTF8_Solid_Wrapped(font, text.data(), foreground, wrapLength);
				break;
			case Sgl::FontRenderType::Shaded:
				surface = TTF_RenderUTF8_Shaded_Wrapped(font, text.data(), foreground, background, wrapLength);
				break;
			case Sgl::FontRenderType::LCD:
				surface = TTF_RenderUTF8_LCD_Wrapped(font, text.data(), foreground, background, wrapLength);
				break;
		}

		_texture = { SDL_CreateTextureFromSurface(GetRenderer(), surface), TextureDeleter() };
		SDL_FreeSurface(surface);

		Log::PrintSDLErrorIf(_texture == nullptr);
	}

	void Texture::SetColor(Color value)
	{
		SDL_SetTextureColorMod(_texture.get(), value.Red, value.Green, value.Blue);
		SDL_SetTextureAlphaMod(_texture.get(), value.Alpha);
	}

	Color Texture::GetColor() const
	{
		Color color = Colors::Transparent;
		SDL_GetTextureColorMod(_texture.get(), &color.Red, &color.Green, &color.Blue);
		SDL_GetTextureAlphaMod(_texture.get(), &color.Alpha);
		return color;
	}

	void Texture::SetBlendMode(SDL_BlendMode value)
	{
		SDL_SetTextureBlendMode(_texture.get(), value);
	}

	SDL_BlendMode Texture::GetBlendMode() const
	{
		SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
		SDL_GetTextureBlendMode(_texture.get(), &blendMode);
		return blendMode;
	}

	void Texture::SetScaleMode(SDL_ScaleMode value)
	{
		SDL_SetTextureScaleMode(_texture.get(), value);
	}

	SDL_ScaleMode Texture::GetScaleMode() const
	{
		SDL_ScaleMode scaleMode = SDL_ScaleModeBest;
		SDL_GetTextureScaleMode(_texture.get(), &scaleMode);
		return scaleMode;
	}

	Size Texture::GetSize() const
	{
		int width = 0, height = 0;
		SDL_QueryTexture(_texture.get(), nullptr, nullptr, &width, &height);
		return Size(width, height);
	}

	TextureAccess Texture::GetAccess() const
	{
		int access = 0;
		SDL_QueryTexture(_texture.get(), nullptr, &access, nullptr, nullptr);
		return TextureAccess(access);
	}

	SDL_PixelFormatEnum Texture::GetFormat() const
	{
		unsigned int format = 0;
		SDL_QueryTexture(_texture.get(), &format, nullptr, nullptr, nullptr);
		return SDL_PixelFormatEnum(format);
	}

	TextureLockContext Texture::Lock(std::optional<Rect> rect)
	{
		return TextureLockContext(*this, rect);
	}	

	TextureLockContext::TextureLockContext(Texture& texture, std::optional<Rect> rect):
		_texture(texture),
		_pixels(),
		_pitch()
	{
		const Rect* lockRect = rect.has_value() ? &rect.value() : nullptr;
		_height = lockRect ? lockRect->h : texture.GetSize().Height;

		if(SDL_LockTexture(_texture.ToSDLTexture(), lockRect, &_pixels, &_pitch) < 0)
		{
			Log::PrintSDLError();
		}
	}

	TextureLockContext::~TextureLockContext()
	{
		if(_pixels)
		{
			SDL_UnlockTexture(_texture.ToSDLTexture());
		}
	}
}
