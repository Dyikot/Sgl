#pragma once

#include "SDL/SDL_mouse.h"
#include "SDL/SDL_image.h"
#include "../Tools/Log.h"
#include <string>
#include "../Tools/Log.h"
#include "../Tools/Log.h"

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
	private:
		static inline Cursors* _instance;

		Cursor _arrow = Cursor(SDL_SYSTEM_CURSOR_ARROW);
		Cursor _ibeam = Cursor(SDL_SYSTEM_CURSOR_IBEAM);
		Cursor _wait = Cursor(SDL_SYSTEM_CURSOR_WAIT);
		Cursor _crosshair = Cursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
		Cursor _waitArrow = Cursor(SDL_SYSTEM_CURSOR_WAITARROW);
		Cursor _arrowNWSE = Cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		Cursor _arrowNESW = Cursor(SDL_SYSTEM_CURSOR_SIZENESW);
		Cursor _arrowWE = Cursor(SDL_SYSTEM_CURSOR_SIZEWE);
		Cursor _arrowNS = Cursor(SDL_SYSTEM_CURSOR_SIZENS);
		Cursor _arrowAll = Cursor(SDL_SYSTEM_CURSOR_SIZEALL);
		Cursor _no = Cursor(SDL_SYSTEM_CURSOR_NO);
		Cursor _hand = Cursor(SDL_SYSTEM_CURSOR_HAND);
	public:
		static void Initialize() { _instance = new Cursors(); }

		static const Cursor& Arrow() { return _instance->_arrow; }
		static const Cursor& Ibeam() { return _instance->_ibeam; }
		static const Cursor& Wait() { return _instance->_wait; }
		static const Cursor& Crosshair() { return _instance->_crosshair; }
		static const Cursor& WaitArrow() { return _instance->_waitArrow; }
		static const Cursor& ArrowNWSE() { return _instance->_arrowNWSE; }
		static const Cursor& ArrowNESW() { return _instance->_arrowNESW; }
		static const Cursor& ArrowWE() { return _instance->_arrowWE; }
		static const Cursor& ArrowNS() { return _instance->_arrowNS; }
		static const Cursor& ArrowAll() { return _instance->_arrowAll; }
		static const Cursor& No() { return _instance->_no; }
		static const Cursor& Hand() { return _instance->_hand; }
	private:
		Cursors() = default;
	};
}