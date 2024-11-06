#pragma once

#include "SDL/SDL_mouse.h"
#include <string>

namespace Sgl
{
	class Cursor
	{
	private:
		SDL_Cursor* _cursor;
	public:
		Cursor() noexcept;
		Cursor(std::string_view path);
		Cursor(SDL_SystemCursor id) noexcept;
		~Cursor() noexcept;

		SDL_Cursor* const SDLCursor()
		{
			return _cursor;
		}
	};
}