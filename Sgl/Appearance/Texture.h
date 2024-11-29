#pragma once

#include <string_view>
#include <iostream>
#include "SDL/SDL_image.h"
#include "Brushes.h"

namespace Sgl
{
	class Texture
	{
	protected:
		SDL_Texture* _texture;
	public:
		Texture(std::string_view path) noexcept;
		Texture(SDL_Texture* texture) noexcept;
		~Texture() noexcept;

		std::pair<size_t, size_t> QuerySize() const noexcept;
		Texture& operator=(SDL_Texture* texture);
		operator SDL_Texture* () const 
		{ 
			return _texture;
		}
	};
}