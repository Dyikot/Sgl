#include "Surface.h"
#include "../Tools/Log.h"

namespace Sgl
{
	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()))
	{
		PrintSDLError();
	}

	Surface::Surface(SDL_Surface* surface):
		_surface(surface)
	{}

	Surface::Surface(const Surface& surface):
		_surface(SDL_DuplicateSurface(surface._surface))
	{
		PrintSDLError();
	}

	Surface::Surface(Surface&& surface) noexcept:
		_surface(surface._surface)
	{
		surface._surface = nullptr;
	}

	std::pair<int, int> Surface::Size() const
	{
		return { _surface->w, _surface->h };
	}

	Surface& Surface::operator=(Surface&& surface) noexcept
	{
		_surface = surface._surface;
		surface._surface = nullptr;
		return *this;
	}

	Surface& Surface::operator=(const Surface& surface) noexcept
	{
		SDL_FreeSurface(_surface);
		_surface = SDL_DuplicateSurface(surface._surface);
		return *this;
	}
}