#include "Surface.h"

namespace Sgl
{
	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()))
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(SDL_Surface* surface):
		_surface(surface)
	{}

	Surface::Surface(Surface && other) noexcept:
		_surface(std::exchange(other._surface, nullptr))
	{}

	Surface::~Surface()
	{
		SDL_FreeSurface(_surface);
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		_surface = std::exchange(other._surface, nullptr);
		return *this;
	}
}
