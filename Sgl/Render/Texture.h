#pragma once

#include <memory>
#include <span>
#include <optional>
#include "../Base/Primitives.h"
#include "../Base/Media/Font.h"
#include "../Base/Size.h"

namespace Sgl
{
	enum class TextureAccess
	{
		Static, Streaming, Target
	};

	class Texture;

	class TextureLockContext
	{
	private:
		Texture& _texture;
		void* _pixels;
		int _pitch;
		int _height;
	public:
		TextureLockContext(Texture& texture, std::optional<Rect> rect);
		~TextureLockContext();

		bool HasLock() const noexcept { return _pixels != nullptr; }
		void* GetPixels() const noexcept { return _pixels; }
		int GetPitch() const noexcept { return _pitch; }

		template<typename T = uint32_t>
		std::span<T> GetPixelsAs()
		{
			if(_pixels == nullptr)
			{
				return {};
			}

			return std::span<T>(
				static_cast<T*>(_pixels),
				static_cast<size_t>(_pitch / sizeof(T) * _height)
			);
		}
	};

	class Texture final
	{
	private:
		std::shared_ptr<SDL_Texture> _texture;
	public:
		Texture() = default;
		Texture(std::nullptr_t);
		explicit Texture(std::string_view path);
		Texture(Size size, 
				TextureAccess access = TextureAccess::Static,
				SDL_PixelFormatEnum format = SDL_PIXELFORMAT_RGBA8888);
		Texture(FontRenderType renderType, 
				TTF_Font* font, 
				std::string_view text,
				Color foreground, 
				Color background = Colors::White);
		Texture(FontRenderType renderType, 
				TTF_Font* font,
				std::string_view text, 
				unsigned int wrapLength,
				Color foreground, 
				Color background = Colors::White);
		Texture(const Texture&) = default;
		Texture(Texture&&) noexcept = default;
		~Texture() = default;

		void SetColor(Color value);
		Color GetColor() const;

		void SetBlendMode(SDL_BlendMode value);
		SDL_BlendMode GetBlendMode() const;

		void SetScaleMode(SDL_ScaleMode value);
		SDL_ScaleMode GetScaleMode() const;

		Size GetSize() const;
		TextureAccess GetAccess() const;
		SDL_PixelFormatEnum GetFormat() const;
		SDL_Texture* ToSDLTexture() const noexcept { return _texture.get(); }

		TextureLockContext Lock(std::optional<Rect> rect = std::nullopt);
		
		Texture& operator=(const Texture&) = default;
		Texture& operator=(Texture&&) noexcept = default;
		friend bool operator==(const Texture&, const Texture&) = default;
		explicit operator bool() { return _texture != nullptr; }
	};
}