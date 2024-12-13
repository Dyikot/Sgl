#pragma once

#include "SDL/SDL_mouse.h"
#include "SDL/SDL_image.h"
#include <string>

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
		{}
		explicit Cursor(SDL_SystemCursor id) noexcept:
			_cursor(SDL_CreateSystemCursor(id))
		{}
		Cursor(const Cursor&) = delete;
		Cursor(Cursor&&) = delete;
		~Cursor() noexcept
		{
			if(_cursor)
			{
				SDL_FreeCursor(_cursor);
			}
		}

		operator SDL_Cursor* () const { return _cursor; }
	};

	class Cursors
	{
	public:
		static inline Cursor Arrow = Cursor(SDL_SYSTEM_CURSOR_ARROW);
		static inline Cursor Ibeam = Cursor(SDL_SYSTEM_CURSOR_IBEAM);
		static inline Cursor Wait = Cursor(SDL_SYSTEM_CURSOR_WAIT);
		static inline Cursor Crosshair = Cursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
		static inline Cursor WaitArrow = Cursor(SDL_SYSTEM_CURSOR_WAITARROW);
		static inline Cursor ArrowNWSE = Cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		static inline Cursor ArrowNESW = Cursor(SDL_SYSTEM_CURSOR_SIZENESW);
		static inline Cursor ArrowWE = Cursor(SDL_SYSTEM_CURSOR_SIZEWE);
		static inline Cursor ArrowNS = Cursor(SDL_SYSTEM_CURSOR_SIZENS);
		static inline Cursor ArrowAll = Cursor(SDL_SYSTEM_CURSOR_SIZEALL);
		static inline Cursor No = Cursor(SDL_SYSTEM_CURSOR_NO);
		static inline Cursor Hand = Cursor(SDL_SYSTEM_CURSOR_HAND);
	};
}