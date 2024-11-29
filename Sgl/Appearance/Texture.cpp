#include "Texture.h"
#include "../Application.h"

namespace Sgl
{
	Texture::Texture(std::string_view path) noexcept:
		_texture(IMG_LoadTexture(Application::Current()->MainWindow->GetRenderContext().Renderer, 
								 path.data()))
	{	
		if(_texture == nullptr)
		{
			PrintSDLError();
			PrintSglError("Error");
		}
	}

	Texture::Texture(SDL_Texture* texture) noexcept:
		_texture(texture) {}

	Texture::~Texture() noexcept
	{
		SDL_DestroyTexture(_texture);
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
}