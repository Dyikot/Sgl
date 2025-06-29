#pragma once

#include <SDL/SDL_mouse.h>
#include <SDL/SDL_image.h>
#include "../Tools/Log.h"
#include "../Tools/Macroses.h"

namespace Sgl
{
	class Cursor
	{
	public:
		using Getter = const Cursor& (*)();
	private:
		SDL_Cursor* _cursor;
	public:
		static void Set(Getter cursorGetter)
		{
			const auto& cursor = cursorGetter();
			if(SDL_GetCursor() != cursor)
			{
				SDL_SetCursor(cursor);
			}
		}

		Cursor() noexcept:
			Cursor(SDL_SYSTEM_CURSOR_ARROW)
		{}

		explicit Cursor(std::string_view path):
			_cursor(SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0))
		{
			Log::PrintSDLErrorIf(!_cursor);
		}

		explicit Cursor(SDL_SystemCursor systemCursor) noexcept:
			_cursor(SDL_CreateSystemCursor(systemCursor))
		{
			Log::PrintSDLErrorIf(!_cursor);
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

		Cursor& operator=(Cursor&& other) noexcept
		{
			_cursor = std::exchange(other._cursor, nullptr);
			return *this;
		}

		operator SDL_Cursor* () const { return _cursor; }
	};

	namespace Cursors
	{
		inline auto Arrow = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_ARROW);
		inline auto IBeam = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_IBEAM);
		inline auto Wait = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_WAIT);
		inline auto Crosshair = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_CROSSHAIR);
		inline auto WaitArrow = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_WAITARROW);
		inline auto ArrowNWSE = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_SIZENWSE);
		inline auto ArrowNESW = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_SIZENESW);
		inline auto ArrowWE = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_SIZEWE);
		inline auto ArrowNS = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_SIZENS);
		inline auto ArrowAll = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_SIZEALL);
		inline auto No = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_NO);
		inline auto Hand = CONST_SINGLETON(Cursor, SDL_SYSTEM_CURSOR_HAND);
	}
}