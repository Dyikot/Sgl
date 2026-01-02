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

	class TextureLock
	{
	private:
		Texture& _texture;
	public:
		void* Pixels = nullptr;
		int Pitch = 0;
	public:
		TextureLock(Texture& texture, const Rect* rect = nullptr);
		~TextureLock();
	};

	class TextureLoader
	{
	private:
		SDL_Renderer*& _renderer;
	public:
		explicit TextureLoader(SDL_Renderer*& renderer): _renderer(renderer) {}

		Texture Load(std::string_view filePath)
		{
			return Texture(_renderer, filePath);
		}
	};
}