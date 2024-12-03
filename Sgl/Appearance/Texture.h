#pragma once

#include "Brushes.h"

namespace Sgl
{
	class Texture
	{
	protected:
		SDL_Texture* _texture;
	public:
		Texture(SDL_Texture* texture) noexcept:
			_texture(texture)
		{}
		Texture(Texture&& texture) noexcept:
			_texture(texture._texture)
		{
			texture._texture = nullptr;
		}
		~Texture() noexcept;

		std::pair<size_t, size_t> QuerySize() const noexcept;
		Texture& operator=(SDL_Texture* texture);
		Texture& operator=(Texture&& texture) noexcept;
		operator SDL_Texture* () const { return _texture; }
	};
}