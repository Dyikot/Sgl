#pragma once

#include <SDL/SDL_mouse.h>
#include <SDL/SDL_image.h>
#include <memory>
#include <variant>
#include <string>

namespace Sgl
{
	class Cursor final
	{
	private:
		mutable SDL_Cursor* _cursor;
		mutable bool _hasTriedToCreate;
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

		friend bool operator==(const Cursor& left, const Cursor& right);
		Cursor& operator=(const Cursor&) = delete;
		Cursor& operator=(Cursor&& other) noexcept;
	private:
		SDL_Cursor* CreateCursor() const;
	};

	namespace Cursors
	{
		inline Cursor Arrow { SDL_SYSTEM_CURSOR_ARROW };
		inline Cursor IBeam { SDL_SYSTEM_CURSOR_IBEAM };
		inline Cursor Wait { SDL_SYSTEM_CURSOR_WAIT };
		inline Cursor Crosshair { SDL_SYSTEM_CURSOR_CROSSHAIR };
		inline Cursor WaitArrow { SDL_SYSTEM_CURSOR_WAITARROW };
		inline Cursor ArrowNWSE { SDL_SYSTEM_CURSOR_SIZENWSE };
		inline Cursor ArrowNESW { SDL_SYSTEM_CURSOR_SIZENESW };
		inline Cursor ArrowWE { SDL_SYSTEM_CURSOR_SIZEWE };
		inline Cursor ArrowNS { SDL_SYSTEM_CURSOR_SIZENS };
		inline Cursor ArrowAll { SDL_SYSTEM_CURSOR_SIZEALL };
		inline Cursor No { SDL_SYSTEM_CURSOR_NO };
		inline Cursor Hand { SDL_SYSTEM_CURSOR_HAND };
	}
}