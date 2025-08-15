#pragma once

#include <SDL/SDL_mouse.h>
#include <SDL/SDL_image.h>
#include <string_view>
#include <memory>

namespace Sgl
{
	class Cursor final
	{
	private:
		std::shared_ptr<SDL_Cursor> _cursor;
	public:		
		Cursor(SDL_SystemCursor systemCursor) noexcept;
		explicit Cursor(std::string_view path);
		Cursor(const Cursor& other);
		Cursor(Cursor&& other) noexcept;
		~Cursor() = default;

		static void Set(const Cursor& cursor);

		SDL_Cursor* ToSDLCursor() const noexcept { return _cursor.get(); }

		Cursor& operator=(const Cursor&) = default;
		Cursor& operator=(Cursor&&) noexcept = default;
		friend bool operator==(const Cursor&, const Cursor&) = default;
	};

	class Cursors
	{
	public:
		static constexpr SDL_SystemCursor Arrow = SDL_SYSTEM_CURSOR_ARROW;
		static constexpr SDL_SystemCursor IBeam = SDL_SYSTEM_CURSOR_IBEAM;
		static constexpr SDL_SystemCursor Wait = SDL_SYSTEM_CURSOR_WAIT;
		static constexpr SDL_SystemCursor Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR;
		static constexpr SDL_SystemCursor WaitArrow = SDL_SYSTEM_CURSOR_WAITARROW;
		static constexpr SDL_SystemCursor SizeNWSE = SDL_SYSTEM_CURSOR_SIZENWSE;
		static constexpr SDL_SystemCursor SizeNESW = SDL_SYSTEM_CURSOR_SIZENESW;
		static constexpr SDL_SystemCursor SizeWE = SDL_SYSTEM_CURSOR_SIZEWE;
		static constexpr SDL_SystemCursor SizeNS = SDL_SYSTEM_CURSOR_SIZENS;
		static constexpr SDL_SystemCursor SizeAll = SDL_SYSTEM_CURSOR_SIZEALL;
		static constexpr SDL_SystemCursor No = SDL_SYSTEM_CURSOR_NO;
		static constexpr SDL_SystemCursor Hand = SDL_SYSTEM_CURSOR_HAND;
	};
}