#pragma once

#include <SDL/SDL_image.h>
#include <SDL/SDL_surface.h>
#include <memory>
#include <string_view>
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
		Surface(uint32_t flags, Size size, int depth, uint32_t format);
		Surface(void* pixels, Size size, int depth, int pitch, uint32_t format);
		Surface(uint32_t flags, Size size, int depth, 
				uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask);
		Surface(void* pixels, Size size, int depth, int pitch,
				uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask);
		Surface(const Surface&) = default;
		Surface(Surface&&) noexcept = default;
		~Surface() = default;

		void SetColor(Color color);
		Color GetColor() const;

		void SetBlendMode(SDL_BlendMode value);
		SDL_BlendMode GetBlendMode() const;

		Size GetSize() const;
		SDL_Surface* ToSDLSurface() const noexcept { return _surface.get(); }

		void Lock();
		void Unlock();

		Surface& operator=(const Surface&) = default	;
		Surface& operator=(Surface&&) noexcept = default;
		friend bool operator==(const Surface&, const Surface&) = default;
		explicit operator bool() { return _surface != nullptr; }
	};
}