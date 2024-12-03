#include "Texture.h"

namespace Sgl
{
	Texture::~Texture() noexcept
	{
		if(_texture)
		{
			SDL_DestroyTexture(_texture);
		}
	}

	std::pair<size_t, size_t> Texture::QuerySize() const noexcept
	{
		int width = 0, height = 0;
		SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
		return std::pair<size_t, size_t>(width, height);
	}

	Texture& Texture::operator=(SDL_Texture* texture)
	{
		SDL_DestroyTexture(_texture);
		_texture = texture;
		return *this;
	}

	Texture& Texture::operator=(Texture&& texture) noexcept
	{
		_texture = texture._texture;
		texture._texture = nullptr;
		return *this;
	}
}