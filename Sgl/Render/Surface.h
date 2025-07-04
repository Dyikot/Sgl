#pragma once

#include <SDL/SDL_image.h>
#include <string_view>
#include "../Tools/Log.h"
#include "../Data/Size.h"
#include "Color.h"

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
			Log::PrintSDLErrorIf(_surface == nullptr);
		}

		explicit Surface(SDL_Surface* surface):
			_surface(surface)
		{}

		Surface(Surface&& other) noexcept:
			_surface(std::exchange(other._surface, nullptr))
		{}

		Surface(const Surface& surface) = delete;

		~Surface() 
		{ 
			SDL_FreeSurface(_surface); 
		}

		void SetColor(Color color)
		{
			SDL_SetSurfaceColorMod(_surface, color.Red, color.Green, color.Blue);
			SDL_SetSurfaceAlphaMod(_surface, color.Alpha);
		}

		Size GetSize() const
		{
			return Size(_surface->w, _surface->h);
		}

		Surface& operator=(Surface&& other) noexcept
		{
			_surface = std::exchange(other._surface, nullptr);
			return *this;
		}

		operator SDL_Surface*() const noexcept { return _surface; }
	};
}