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
		static const auto& Arrow() { static Cursor c(SDL_SYSTEM_CURSOR_ARROW); return c; }
		static const auto& IBeam() { static Cursor c(SDL_SYSTEM_CURSOR_IBEAM); return c; }
		static const auto& Wait() { static Cursor c(SDL_SYSTEM_CURSOR_WAIT); return c; }
		static const auto& Crosshair() { static Cursor c(SDL_SYSTEM_CURSOR_CROSSHAIR); return c; }
		static const auto& WaitArrow() { static Cursor c(SDL_SYSTEM_CURSOR_WAITARROW); return c; }
		static const auto& ArrowNWSE() { static Cursor c(SDL_SYSTEM_CURSOR_SIZENWSE); return c; }
		static const auto& ArrowNESW() { static Cursor c(SDL_SYSTEM_CURSOR_SIZENESW); return c; }
		static const auto& ArrowWE() { static Cursor c(SDL_SYSTEM_CURSOR_SIZEWE); return c; }
		static const auto& ArrowNS() { static Cursor c(SDL_SYSTEM_CURSOR_SIZENS); return c; }
		static const auto& ArrowAll() { static Cursor c(SDL_SYSTEM_CURSOR_SIZEALL); return c; }
		static const auto& No() { static Cursor c(SDL_SYSTEM_CURSOR_NO); return c; }
		static const auto& Hand() { static Cursor c(SDL_SYSTEM_CURSOR_HAND); return c; }
	};
}