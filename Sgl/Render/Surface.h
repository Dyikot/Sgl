#pragma once

#include <SDL/SDL_image.h>
#include <string_view>
#include "../Base/Color.h"
#include "../Base/Log.h"
#include "../Base/Size.h"

namespace Sgl
{
	class Surface
	{
	private:
		SDL_Surface* _surface = nullptr;
	public:
		explicit Surface(std::string_view path);
		explicit Surface(SDL_Surface* surface);
		Surface(Surface&& other) noexcept;
		Surface(const Surface&) = delete;
		~Surface();

		Size GetSize() const { return Size(_surface->w, _surface->h); }
		SDL_Surface* GetSDLSurface() const noexcept { return _surface; }

		void SetColor(Color color)
		{
			SDL_SetSurfaceColorMod(_surface, color.Red, color.Green, color.Blue);
			SDL_SetSurfaceAlphaMod(_surface, color.Alpha);
		}

		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&& other) noexcept;
	};
}