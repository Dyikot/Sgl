#pragma once

#include "Brushes.h"

namespace Sgl
{
	class Texture
	{
	protected:
		SDL_Texture* _texture;
	public:
		Texture(SDL_Texture* texture) noexcept;
		~Texture() noexcept;

		std::pair<size_t, size_t> QuerySize() const noexcept;
		Texture& operator=(SDL_Texture* texture);
		operator SDL_Texture* () const { return _texture; }
	};
}