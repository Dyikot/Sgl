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
		explicit Surface(std::string_view path);
		Surface(const Surface& surface) = delete;
		Surface(Surface&& other) noexcept;
		~Surface() noexcept;

		std::pair<size_t, size_t> Size() const;

		operator SDL_Surface* () const { return _surface; }
		Surface& operator=(Surface&& other) noexcept;
	};
}