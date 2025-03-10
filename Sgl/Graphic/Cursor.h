#pragma once

#include "SDL/SDL_mouse.h"
#include "SDL/SDL_image.h"
#include "../Tools/Log.h"
#include <string>
#include <optional>

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

		Cursor(Cursor&& cursor) noexcept:
			_cursor(cursor._cursor)
		{
			cursor._cursor = nullptr;
		}

		~Cursor() noexcept
		{
			if(_cursor)
			{
				SDL_FreeCursor(_cursor);
			}
		}

		operator SDL_Cursor* () const { return _cursor; }
		Cursor& operator=(Cursor&& cursor) noexcept
		{
			_cursor = cursor._cursor;
			cursor._cursor = nullptr;
			return *this;
		}
	};

	class Cursors
	{
	private:
		static inline std::optional<Cursor> _arrow = std::nullopt;
		static inline std::optional<Cursor> _ibeam = std::nullopt;
		static inline std::optional<Cursor> _wait = std::nullopt;
		static inline std::optional<Cursor> _crosshair = std::nullopt;
		static inline std::optional<Cursor> _waitArrow = std::nullopt;
		static inline std::optional<Cursor> _arrowNWSE = std::nullopt;
		static inline std::optional<Cursor> _arrowNESW = std::nullopt;
		static inline std::optional<Cursor> _arrowWE = std::nullopt;
		static inline std::optional<Cursor> _arrowNS = std::nullopt;
		static inline std::optional<Cursor> _arrowAll = std::nullopt;
		static inline std::optional<Cursor> _no = std::nullopt;
		static inline std::optional<Cursor> _hand = std::nullopt;
	public:
		static const Cursor& Arrow()
		{
			if(!_arrow)
			{
				_arrow = Cursor(SDL_SYSTEM_CURSOR_ARROW);
			}

			return _arrow.value();
		}

		static const Cursor& Ibeam()
		{
			if(!_ibeam)
			{
				_ibeam = Cursor(SDL_SYSTEM_CURSOR_IBEAM);
			}

			return _ibeam.value();
		}

		static const Cursor& Wait()
		{
			if(!_wait)
			{
				_wait = Cursor(SDL_SYSTEM_CURSOR_WAIT);
			}

			return _wait.value();
		}

		static const Cursor& Crosshair()
		{
			if(!_crosshair)
			{
				_crosshair = Cursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
			}

			return _crosshair.value();
		}

		static const Cursor& WaitArrow()
		{
			if(!_waitArrow)
			{
				_waitArrow = Cursor(SDL_SYSTEM_CURSOR_WAITARROW);
			}

			return _waitArrow.value();
		}

		static const Cursor& ArrowNWSE()
		{
			if(!_arrowNWSE)
			{
				_arrowNWSE = Cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
			}

			return _arrowNWSE.value();
		}

		static const Cursor& ArrowNESW()
		{
			if(!_arrowNESW)
			{
				_arrowNESW = Cursor(SDL_SYSTEM_CURSOR_SIZENESW);
			}

			return _arrowNESW.value();
		}

		static const Cursor& ArrowWE()
		{
			if(!_arrowWE)
			{
				_arrowWE = Cursor(SDL_SYSTEM_CURSOR_SIZEWE);
			}

			return _arrowWE.value();
		}

		static const Cursor& ArrowNS()
		{
			if(!_arrowNS)
			{
				_arrowNS = Cursor(SDL_SYSTEM_CURSOR_SIZENS);
			}

			return _arrowNS.value();
		}

		static const Cursor& ArrowAll()
		{
			if(!_arrowAll)
			{
				_arrowAll = Cursor(SDL_SYSTEM_CURSOR_SIZEALL);
			}

			return _arrowAll.value();
		}

		static const Cursor& No()
		{
			if(!_no)
			{
				_no = Cursor(SDL_SYSTEM_CURSOR_NO);
			}

			return _no.value();
		}

		static const Cursor& Hand()
		{
			if(!_hand)
			{
				_hand = Cursor(SDL_SYSTEM_CURSOR_HAND);
			}

			return _hand.value();
		}
	};
}