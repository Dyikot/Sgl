#pragma once

#include <SDL3/SDL_mouse.h>
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
		static void Show();
		static void Hide();
		static bool IsVisible();

		SDL_Cursor* GetSDLCursor() const noexcept { return _cursor.get(); }

		Cursor& operator=(const Cursor&) = default;
		Cursor& operator=(Cursor&&) noexcept = default;
		friend bool operator==(const Cursor&, const Cursor&) = default;
	};

	class Cursors
	{
	public:
		static constexpr SDL_SystemCursor Arrow = SDL_SYSTEM_CURSOR_DEFAULT;
		static constexpr SDL_SystemCursor IBeam = SDL_SYSTEM_CURSOR_TEXT;
		static constexpr SDL_SystemCursor Wait = SDL_SYSTEM_CURSOR_WAIT;
		static constexpr SDL_SystemCursor Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR;
		static constexpr SDL_SystemCursor Progress = SDL_SYSTEM_CURSOR_PROGRESS;
		static constexpr SDL_SystemCursor ResizeNWSE = SDL_SYSTEM_CURSOR_NWSE_RESIZE;
		static constexpr SDL_SystemCursor ResizeNESW = SDL_SYSTEM_CURSOR_NESW_RESIZE;
		static constexpr SDL_SystemCursor ResizeEW = SDL_SYSTEM_CURSOR_EW_RESIZE;
		static constexpr SDL_SystemCursor ResizeNS = SDL_SYSTEM_CURSOR_NS_RESIZE;
		static constexpr SDL_SystemCursor Move = SDL_SYSTEM_CURSOR_MOVE;
		static constexpr SDL_SystemCursor NotAllowed = SDL_SYSTEM_CURSOR_NOT_ALLOWED;
		static constexpr SDL_SystemCursor Pointer = SDL_SYSTEM_CURSOR_POINTER;
	};
}