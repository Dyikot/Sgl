#pragma once
#include <SDL/SDL_image.h>
#include <string_view>
#include "../Tools/Log.h"

namespace Sgl
{
	class Surface
	{
	private:
		SDL_Surface* _surface = nullptr;
	public:
		explicit Surface(std::string_view path):
			_surface(IMG_Load(path.data()))
		{
			if(_surface == nullptr)
			{
				Log::PrintSDLError();
			}
		}

		Surface(const Surface& surface) = delete;

		Surface(Surface&& other) noexcept:
			_surface(std::exchange(other._surface, nullptr))
		{}

		~Surface() noexcept
		{
			if(_surface)
			{
				SDL_FreeSurface(_surface);
			}
		}

		std::pair<size_t, size_t> Size() const
		{
			return std::pair{ _surface->w, _surface->h };
		}

		operator SDL_Surface*() const
		{ 
			return _surface;
		}

		Surface& operator=(Surface&& other) noexcept
		{
			_surface = std::exchange(other._surface, nullptr);
			return *this;
		}
	};
}