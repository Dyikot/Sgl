#pragma once

#include <string_view>
#include <SDL3/SDL_surface.h>

#include "../Base/Size.h"
#include "../Base/Media/Color.h"

namespace Sgl
{
	class Surface final
	{
	private:
		SDL_Surface* _surface = nullptr;
	public:
		Surface() = default;
		Surface(std::nullptr_t);
		explicit Surface(std::string_view path);
		explicit Surface(SDL_Surface* sdlSurface);
		Surface(Size size, SDL_PixelFormat format);
		Surface(Size size, SDL_PixelFormat format, void* pixels, int pitch);
		Surface(const Surface& other);
		Surface(Surface&& other) noexcept;
		~Surface();

		void SetColor(Color color);
		Color GetColor() const;

		void SetBlendMode(SDL_BlendMode value);
		SDL_BlendMode GetBlendMode() const;

		size_t GetWidth() const;
		size_t GetHeight() const;
		SDL_Surface* GetSDLSurface() const noexcept { return _surface; }

		void Lock();
		void Unlock();
		void Flip(SDL_FlipMode flipMode);
		Surface Clone() const;

		Surface& operator=(std::nullptr_t);
		Surface& operator=(const Surface& other);
		Surface& operator=(Surface&& other) noexcept;
		friend bool operator==(const Surface&, const Surface&) = default;
		bool operator==(std::nullptr_t) const noexcept { return _surface == nullptr; }
		explicit operator bool() const noexcept { return _surface != nullptr; }
	private:
		void CopyFrom(const Surface& other);
		void Destroy();
	};
}