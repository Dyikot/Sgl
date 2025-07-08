#pragma once

#include <utility>
#include <string_view>
#include <SDL/SDL_image.h>
#include "../Base/Color.h"
#include "../Base/Log.h"
#include "../Base/Primitives.h"
#include "../Base/Size.h"

namespace Sgl
{
	enum class TextureAccess
	{
		Static, Streaming, Target
	};

	enum class TextureFlip
	{
		Horizontal = 1, Vertical = 2
	};

	class Texture
	{
	protected:
		SDL_Texture* _texture = nullptr;
	public:
		Texture() = default;
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