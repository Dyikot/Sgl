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
			PrintSDLErrorIf(_surface == nullptr);
		}

		Surface(Surface&& other) noexcept:
			_surface(std::exchange(other._surface, nullptr))
		{}

		Surface(const Surface& surface) = delete;
		~Surface() { SDL_FreeSurface(_surface); }

		auto Size() const
		{
			return std::pair<size_t, size_t>(_surface->w, _surface->h);
		}

		Surface& operator=(Surface&& other) noexcept
		{
			_surface = std::exchange(other._surface, nullptr);
			return *this;
		}

		operator SDL_Surface*() const noexcept 
		{ 
			return _surface;
		}
	};
}