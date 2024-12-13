#pragma once
#include <SDL/SDL_image.h>
#include <string_view>

namespace Sgl
{
	class Surface
	{
	private:
		SDL_Surface* _surface = nullptr;
	public:
		Surface() = delete;
		explicit Surface(std::string_view path);
		explicit Surface(SDL_Surface* surface);
		Surface(const Surface& surface);
		Surface(Surface&& surface) noexcept;
		~Surface() noexcept { SDL_FreeSurface(_surface); }

		std::pair<int, int> Size() const;

		operator SDL_Surface* () const { return _surface; }
		Surface& operator=(Surface&& surface) noexcept;
		Surface& operator=(const Surface& surface) noexcept;
	};
}