#include "Texture.h"
#include "../Application.h"

namespace Sgl
{
	Texture::Texture(std::string_view path) noexcept:
		_texture(IMG_LoadTexture(Application::Current()->MainWindow->GetRenderContext().Renderer, path.data()))
	{	
		if(_texture == nullptr)
		{
			std::cout << SDL_GetError() << '\n';
		}
	}

	Texture::Texture(SDL_Texture* texture) noexcept:
		_texture(texture) {}

	Texture::~Texture() noexcept
	{
		SDL_DestroyTexture(_texture);
	}

	Texture::Size Texture::QuerySize() const noexcept
	{
		Size size = {};
		SDL_QueryTexture(_texture, NULL, NULL, &size.Width, &size.Height);
		return size;
	}

	Texture& Texture::operator=(SDL_Texture* texture)
	{
		SDL_DestroyTexture(_texture);
		_texture = texture;

		return *this;
	}
}