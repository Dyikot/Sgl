#include "Texture.h"

namespace Sgl
{
	Texture::Texture(SDL_Texture* texture) noexcept:
		_texture(texture)
	{}

	Texture::Texture(Texture&& other) noexcept:
		_texture(std::exchange(other._texture, nullptr))
	{}

	Texture::~Texture() noexcept
	{
		if(_texture)
		{
			SDL_DestroyTexture(_texture);
		}
	}

	std::pair<size_t, size_t> Texture::Size() const
	{
		int width = 0, height = 0;
		SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
		return { width, height };
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		_texture = std::exchange(other._texture, nullptr);
		return *this;
	}
}