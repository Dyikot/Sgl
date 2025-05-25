#pragma once

#include "SDL/SDL_mouse.h"
#include "SDL/SDL_image.h"
#include "../Tools/Log.h"
#include "../Data/Singleton.h"

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

		operator SDL_Cursor* () const 
		{ 
			return _cursor;
		}
	};

	namespace Cursors
	{
		inline auto Arrow = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_ARROW);
		inline auto IBeam = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_IBEAM);
		inline auto Wait = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_WAIT);
		inline auto Crosshair = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_CROSSHAIR);
		inline auto WaitArrow = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_WAITARROW);
		inline auto ArrowNWSE = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_SIZENWSE);
		inline auto ArrowNESW = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_SIZENESW);
		inline auto ArrowWE = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_SIZEWE);
		inline auto ArrowNS = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_SIZENS);
		inline auto ArrowAll = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_SIZEALL);
		inline auto No = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_NO);
		inline auto Hand = Sgl_ConstSingleton(Cursor, SDL_SYSTEM_CURSOR_HAND);
	}
}