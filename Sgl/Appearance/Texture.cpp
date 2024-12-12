#include "Texture.h"

namespace Sgl
{
	Texture::Texture(SDL_Texture* texture) noexcept:
		_texture(texture)
	{}

	Texture::Texture(Texture&& texture) noexcept:
		_texture(texture._texture)
	{
		texture._texture = nullptr;
	}

	Texture::~Texture() noexcept
	{
		if(_texture)
		{
			SDL_DestroyTexture(_texture);
		}
	}

	std::pair<int, int> Texture::Size() const
	{
		int width = 0, height = 0;
		SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
		return { width, height };
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