#pragma once

#include <utility>
#include <string_view>
#include "../Tools/Log.h"
#include "SDL/SDL_render.h"
#include "SDL/SDL_image.h"

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

	struct TextureFactory
	{
	private:
		SDL_Renderer* _renderer;
	public:
		TextureFactory(SDL_Renderer* renderer) noexcept:
			_renderer(renderer)
		{}

		Texture operator()(std::string_view path) const noexcept
		{
			auto texture = IMG_LoadTexture(_renderer, path.data());
			PRINT_SDL_ERROR_IF(texture == nullptr);

			return Texture(texture);
		}
	};
}