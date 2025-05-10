#pragma once

#include <utility>
#include <string_view>
#include "../Tools/Log.h"
#include "SDL/SDL_render.h"
#include "SDL/SDL_image.h"

namespace Sgl
{
	class Texture
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		explicit Texture(SDL_Texture* texture) noexcept:
			_texture(texture)
		{}

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