#include "Texture.h"

namespace Sgl
{
	Texture::Texture(SDL_Texture* texture):
		_texture(texture)
	{
		Log::PrintSDLErrorIf(_texture == nullptr);
	}

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

	Size Texture::GetSize() const
	{
		int width = 0, height = 0;
		SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
		return Size(width, height);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		_texture = std::exchange(other._texture, nullptr);
		return *this;
	}
}
