#pragma once

#include "SDL/SDL_mouse.h"
#include "SDL/SDL_image.h"
#include <string>

namespace Sgl
{
	class Cursor
	{
	private:
		SDL_Cursor* _surface;
	public:
		Cursor() noexcept:
			Cursor(SDL_SYSTEM_CURSOR_ARROW)
		{}
		Cursor(std::string_view path):
			_surface(SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0))
		{}
		Cursor(SDL_SystemCursor id) noexcept:
			_surface(SDL_CreateSystemCursor(id))
		{}
		Cursor(Cursor&& cursor) noexcept:
			_surface(cursor._surface)
		{
			cursor._surface = nullptr;
		}
		~Cursor() noexcept
		{
			if(_surface)
			{
				SDL_FreeCursor(_surface);
			}
		}

		operator SDL_Cursor* () const { return _surface; }
		Cursor& operator=(Cursor&& cursor) noexcept
		{
			_surface = cursor._surface;
			cursor._surface = nullptr;
			return *this;
		}
	};
}