#pragma once

#include <utility>
#include "Surface.h"
#include "RenderContext.h"

namespace Sgl
{
	enum class TextureAccess
	{
		Static, Streaming, Target
	};

	class Texture
	{
	protected:
		SDL_Texture* _texture = nullptr;
	public:
		explicit Texture(std::string_view path);
		explicit Texture(const Surface& surface);
		Texture(TextureAccess textureAccess, Size size);
		Texture(TextureAccess textureAccess, Size size, RenderFragment renderFragment);
		Texture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, Size size);
		Texture(SDL_PixelFormatEnum pixelFormat, TextureAccess textureAccess, Size size, RenderFragment renderFragment);
		Texture(const std::string& text, const Font& font);
		Texture(SDL_Texture* texture);
		Texture(const Texture&) = delete;
		Texture(Texture&& other) noexcept;
		~Texture() noexcept;

		SDL_Texture* GetSDLTexture() const noexcept { return _texture; }
		Size GetSize() const;

		void SetColor(Color color) const
		{
			SDL_SetTextureColorMod(_texture, color.Red, color.Green, color.Blue);
			SDL_SetTextureAlphaMod(_texture, color.Alpha);
		}

		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& other) noexcept;
	};
}