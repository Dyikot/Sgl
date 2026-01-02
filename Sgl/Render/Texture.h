#pragma once

#include <span>
#include <SDL3/SDL_render.h>

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
		TextureLockContext(Texture& texture, const Rect* rect);
		~TextureLockContext();

		bool HasLock() const noexcept;
		void* GetPixels() const noexcept;
		int GetPitch() const noexcept;

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
		SDL_Texture* _texture = nullptr;
	public:
		Texture() = default;
		Texture(std::nullptr_t);
		Texture(SDL_Renderer* renderer, std::string_view filePath);
		Texture(SDL_Renderer* renderer,
				Size size, 
				TextureAccess access = TextureAccess::Static,
				SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);
		Texture(SDL_Renderer* renderer,
				FontQuality fontQuality,
				TTF_Font* font, 
				std::string_view text,
				Color foreground, 
				Color background = Colors::Transparent);
		Texture(SDL_Renderer* renderer,
				FontQuality fontQuality, 
				TTF_Font* font,
				std::string_view text, 
				int wrapWidth,
				Color foreground, 
				Color background = Colors::Transparent);
		Texture(const Texture& other);
		Texture(Texture&& other) noexcept;
		~Texture();

		void SetColor(Color value);
		Color GetColor() const;

		void SetBlendMode(SDL_BlendMode value);
		SDL_BlendMode GetBlendMode() const;

		void SetScaleMode(SDL_ScaleMode value);
		SDL_ScaleMode GetScaleMode() const;

		size_t GetWidth() const;
		size_t GetHeight() const;
		TextureAccess GetAccess() const;
		SDL_PixelFormat GetFormat() const;
		SDL_Texture* GetSDLTexture() const noexcept;

		TextureLockContext Lock(const Rect* rect = nullptr);
		
		Texture& operator=(std::nullptr_t);
		Texture& operator=(const Texture& other);
		Texture& operator=(Texture&& other) noexcept;
		friend bool operator==(const Texture&, const Texture&) = default;
		bool operator==(std::nullptr_t) const noexcept { return _texture == nullptr; }
		explicit operator bool() const noexcept { return _texture != nullptr; }
	private:
		void CopyFrom(const Texture& other);
		void Destroy();
	};
}