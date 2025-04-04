#pragma once

#include <utility>
#include "SDL/SDL_render.h"

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

		Texture(Texture&& other) noexcept:
			_texture(std::exchange(other._texture, nullptr))
		{}

		Texture(const Texture&) = delete;

		~Texture() noexcept
		{
			if(_texture)
			{
				SDL_DestroyTexture(_texture);
			}
		}

		std::pair<size_t, size_t> Size() const
		{
			int width = 0, height = 0;
			SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
			return { width, height };
		}

		operator SDL_Texture* () const 
		{
			return _texture; 
		}

		Texture& operator=(Texture&& other) noexcept
		{
			_texture = std::exchange(other._texture, nullptr);
			return *this;
		}
	};
}