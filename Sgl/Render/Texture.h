#pragma once

#include <utility>
#include <string_view>
#include "../Tools/Log.h"
#include "SDL/SDL_image.h"
#include "Primitives.h"
#include "Color.h"
#include "../Data/Helpers.h"

namespace Sgl
{
	enum class TextureAccess
	{
		Static, Streaming, Target
	};

	class Texture
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		Texture() = default;

		Texture(SDL_Texture* texture):
			_texture(texture)
		{
			Log::PrintSDLErrorIf(_texture == nullptr);
		}

		Texture(const Texture&) = delete;

		Texture(Texture&& other) noexcept:
			_texture(std::exchange(other._texture, nullptr))
		{}

		~Texture() noexcept
		{
			if(_texture)
			{
				SDL_DestroyTexture(_texture);
			}
		}

		void SetColor(Color color) const
		{
			SDL_SetTextureColorMod(_texture, color.Red, color.Green, color.Blue);
			SDL_SetTextureAlphaMod(_texture, color.Alpha);
		}

		Size GetSize() const
		{
			int width = 0, height = 0;
			SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
			return Size(width, height);
		}

		bool IsEmpty() const noexcept
		{
			return _texture == nullptr;
		}

		Texture& operator=(Texture&& other) noexcept
		{
			_texture = std::exchange(other._texture, nullptr);
			return *this;
		}

		operator SDL_Texture* () const noexcept 
		{ 
			return _texture; 
		}
	};
}