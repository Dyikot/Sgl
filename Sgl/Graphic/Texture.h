#pragma once

#include <utility>
#include <string_view>
#include "../Tools/Log.h"
#include "SDL/SDL_image.h"
#include "Primitives.h"
#include "Color.h"

namespace Sgl
{
	class Texture
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		Texture(std::string_view path, SDL_Renderer* renderer):
			_texture(IMG_LoadTexture(renderer, path.data()))
		{			
			PrintSDLErrorIf(_texture == nullptr);
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

		void SetColor(Color color)
		{
			SDL_SetTextureColorMod(_texture, color.Red, color.Green, color.Blue);
			SDL_SetTextureAlphaMod(_texture, color.Alpha);
		}

		auto Size() const
		{
			int width = 0, height = 0;
			SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
			return std::pair<size_t, size_t>(width, height);
		}

		Texture& operator=(Texture&& other) noexcept
		{
			_texture = std::exchange(other._texture, nullptr);
			return *this;
		}

		operator SDL_Texture* () noexcept 
		{ 
			return _texture; 
		}

		operator const SDL_Texture* () const noexcept
		{
			return _texture;
		}
	};
}