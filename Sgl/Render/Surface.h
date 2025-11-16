#pragma once

#include <memory>
#include <string_view>
#include <SDL3/SDL_surface.h>
#include "../Base/Size.h"
#include "../Base/Media/Color.h"

namespace Sgl
{
	class Surface final
	{
	private:
		std::shared_ptr<SDL_Surface> _surface;
	public:
		Surface() = default;
		Surface(std::nullptr_t);
		explicit Surface(std::string_view path);
		explicit Surface(SDL_Surface* sdlSurface);
		Surface(Size size, SDL_PixelFormat format);
		Surface(Size size, SDL_PixelFormat format, void* pixels, size_t pitch);
		Surface(const Surface&) = default;
		Surface(Surface&&) noexcept = default;
		~Surface() = default;

		void SetColor(Color color);
		Color GetColor() const;

		void SetBlendMode(SDL_BlendMode value);
		SDL_BlendMode GetBlendMode() const;

		Size GetSize() const;
		SDL_Surface* GetSDLSurface() const noexcept { return _surface.get(); }

		void Lock();
		void Unlock();
		void Flip(SDL_FlipMode flipMode);
		Surface Clone() const;

		Surface& operator=(const Surface&) = default	;
		Surface& operator=(Surface&&) noexcept = default;
		friend bool operator==(const Surface&, const Surface&) = default;
		explicit operator bool() { return _surface != nullptr; }
	};
}