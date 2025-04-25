#pragma once

#include "SDL/SDL_mouse.h"
#include "SDL/SDL_image.h"
#include "../Tools/Log.h"
#include "../Data/Lazy.h"
#include <string_view>

namespace Sgl
{
	class Cursor
	{
	private:
		SDL_Cursor* _cursor;
	public:
		Cursor() noexcept:
			Cursor(SDL_SYSTEM_CURSOR_ARROW)
		{}

		explicit Cursor(std::string_view path):
			_cursor(SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0))
		{
			PrintSDLErrorIf(!_cursor);
		}

		explicit Cursor(SDL_SystemCursor id) noexcept:
			_cursor(SDL_CreateSystemCursor(id))
		{
			PrintSDLErrorIf(!_cursor);
		}

		Cursor(const Cursor&) = delete;
		Cursor(Cursor&& other) noexcept:
			_cursor(std::exchange(other._cursor, nullptr))
		{}

		~Cursor() noexcept
		{
			if(_cursor)
			{
				SDL_FreeCursor(_cursor);
			}
		}

		operator SDL_Cursor* () const { return _cursor; }

		Cursor& operator=(Cursor&& other) noexcept
		{
			_cursor = std::exchange(other._cursor, nullptr);
			return *this;
		}
	};

	class Cursors
	{
	public:
		static inline Lazy<Cursor> arrow = [] { return Cursor(SDL_SYSTEM_CURSOR_ARROW); };
		static inline Lazy<Cursor> iBeam = [] { return Cursor(SDL_SYSTEM_CURSOR_IBEAM); };
		static inline Lazy<Cursor> wait = [] { return Cursor(SDL_SYSTEM_CURSOR_WAIT); };
		static inline Lazy<Cursor> crosshair = [] { return Cursor(SDL_SYSTEM_CURSOR_CROSSHAIR); };
		static inline Lazy<Cursor> waitArrow = [] { return Cursor(SDL_SYSTEM_CURSOR_WAITARROW); };
		static inline Lazy<Cursor> arrowNWSE = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZENWSE); };
		static inline Lazy<Cursor> arrowNESW = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZENESW); };
		static inline Lazy<Cursor> arrowWE = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZEWE); };
		static inline Lazy<Cursor> arrowNS = [] { return  Cursor(SDL_SYSTEM_CURSOR_SIZENS); };
		static inline Lazy<Cursor> arrowAll = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZEALL); };
		static inline Lazy<Cursor> no = [] { return Cursor(SDL_SYSTEM_CURSOR_NO); };
		static inline Lazy<Cursor> hand = [] { return Cursor(SDL_SYSTEM_CURSOR_HAND); };
	};
}