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
			if(!_cursor)
			{
				Log::PrintSDLError();
			}
		}

		explicit Cursor(SDL_SystemCursor id) noexcept:
			_cursor(SDL_CreateSystemCursor(id))
		{
			if(!_cursor)
			{
				Log::PrintSDLError();
			}
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
		static inline Lazy Arrow = [] { return Cursor(SDL_SYSTEM_CURSOR_ARROW); };
		static inline Lazy IBeam = [] { return Cursor(SDL_SYSTEM_CURSOR_IBEAM); };
		static inline Lazy Wait = [] { return Cursor(SDL_SYSTEM_CURSOR_WAIT); };
		static inline Lazy Crosshair = [] { return Cursor(SDL_SYSTEM_CURSOR_CROSSHAIR); };
		static inline Lazy WaitArrow = [] { return Cursor(SDL_SYSTEM_CURSOR_WAITARROW); };
		static inline Lazy ArrowNWSE = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZENWSE); };
		static inline Lazy ArrowNESW = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZENESW); };
		static inline Lazy ArrowWE = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZEWE); };
		static inline Lazy ArrowNS = [] { return  Cursor(SDL_SYSTEM_CURSOR_SIZENS); };
		static inline Lazy ArrowAll = [] { return Cursor(SDL_SYSTEM_CURSOR_SIZEALL); };
		static inline Lazy No = [] { return Cursor(SDL_SYSTEM_CURSOR_NO); };
		static inline Lazy Hand = [] { return Cursor(SDL_SYSTEM_CURSOR_HAND); };
	};
}