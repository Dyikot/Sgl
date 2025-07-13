#pragma once

#include <SDL/SDL_mouse.h>
#include <SDL/SDL_image.h>
#include <memory>
#include <variant>
#include <string>

namespace Sgl
{
	class Cursor
	{
	private:
		mutable SDL_Cursor* _cursor;
		std::variant<std::string, SDL_SystemCursor> _creationArgs;
	public:
		static void Set(const Cursor& cursor);

		Cursor() noexcept;
		explicit Cursor(std::string path);
		explicit Cursor(SDL_SystemCursor systemCursor) noexcept;

		Cursor(const Cursor&) = delete;
		Cursor(Cursor&& other) noexcept;
		~Cursor() noexcept;

		SDL_Cursor* GetSDLCursor() const noexcept;

		Cursor& operator=(const Cursor&) = delete;
		Cursor& operator=(Cursor&& other) noexcept;
	private:
		SDL_Cursor* CreateCursor() const;
	};

	namespace Cursors
	{
		inline const Cursor Arrow = Cursor(SDL_SYSTEM_CURSOR_ARROW);
		inline const Cursor IBeam = Cursor(SDL_SYSTEM_CURSOR_IBEAM);
		inline const Cursor Wait = Cursor(SDL_SYSTEM_CURSOR_WAIT);
		inline const Cursor Crosshair = Cursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
		inline const Cursor WaitArrow = Cursor(SDL_SYSTEM_CURSOR_WAITARROW);
		inline const Cursor ArrowNWSE = Cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		inline const Cursor ArrowNESW = Cursor(SDL_SYSTEM_CURSOR_SIZENESW);
		inline const Cursor ArrowWE = Cursor(SDL_SYSTEM_CURSOR_SIZEWE);
		inline const Cursor ArrowNS = Cursor(SDL_SYSTEM_CURSOR_SIZENS);
		inline const Cursor ArrowAll = Cursor(SDL_SYSTEM_CURSOR_SIZEALL);
		inline const Cursor No = Cursor(SDL_SYSTEM_CURSOR_NO);
		inline const Cursor Hand = Cursor(SDL_SYSTEM_CURSOR_HAND);
	}
}