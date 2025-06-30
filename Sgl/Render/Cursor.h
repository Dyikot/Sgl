#pragma once

#include <SDL/SDL_mouse.h>
#include <SDL/SDL_image.h>
#include <memory>
#include <variant>
#include <string_view>

namespace Sgl
{
	class Cursor
	{
	private:
		mutable SDL_Cursor* _cursor;
		std::variant<std::string_view, SDL_SystemCursor> _creationArgs;
	public:
		static void Set(std::shared_ptr<Cursor> cursor);
		static std::shared_ptr<Cursor> New() { return std::make_shared<Cursor>(); }
		static std::shared_ptr<Cursor> New(std::string_view path) { return std::make_shared<Cursor>(path); }
		static std::shared_ptr<Cursor> New(SDL_SystemCursor systemCursor) { return std::make_shared<Cursor>(systemCursor); }

		Cursor() noexcept;
		explicit Cursor(std::string_view path);
		explicit Cursor(SDL_SystemCursor systemCursor) noexcept;

		Cursor(const Cursor&) = delete;
		Cursor(Cursor&& other) noexcept;
		~Cursor() noexcept;

		SDL_Cursor* GetSDLCursor() const;

		Cursor& operator=(const Cursor&) = delete;
		Cursor& operator=(Cursor&& other) noexcept;
	private:
		SDL_Cursor* CreateCursor() const;
	};

	namespace Cursors
	{
		inline auto Arrow = Cursor::New(SDL_SYSTEM_CURSOR_ARROW);
		inline auto IBeam = Cursor::New(SDL_SYSTEM_CURSOR_IBEAM);
		inline auto Wait = Cursor::New(SDL_SYSTEM_CURSOR_WAIT);
		inline auto Crosshair = Cursor::New(SDL_SYSTEM_CURSOR_CROSSHAIR);
		inline auto WaitArrow = Cursor::New(SDL_SYSTEM_CURSOR_WAITARROW);
		inline auto ArrowNWSE = Cursor::New(SDL_SYSTEM_CURSOR_SIZENWSE);
		inline auto ArrowNESW = Cursor::New(SDL_SYSTEM_CURSOR_SIZENESW);
		inline auto ArrowWE = Cursor::New(SDL_SYSTEM_CURSOR_SIZEWE);
		inline auto ArrowNS = Cursor::New(SDL_SYSTEM_CURSOR_SIZENS);
		inline auto ArrowAll = Cursor::New(SDL_SYSTEM_CURSOR_SIZEALL);
		inline auto No = Cursor::New(SDL_SYSTEM_CURSOR_NO);
		inline auto Hand = Cursor::New(SDL_SYSTEM_CURSOR_HAND);
	}
}