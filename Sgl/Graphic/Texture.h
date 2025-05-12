#pragma once

#include <utility>
#include <string_view>
#include "../Tools/Log.h"
#include "SDL/SDL_image.h"
#include "Types.h"

namespace Sgl
{
	class Texture
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		Texture(std::string_view path, Renderer renderer):
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

		operator SDL_Texture* () const noexcept 
		{ 
			return _texture; 
		}
	};
}