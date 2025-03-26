#include "Surface.h"
#include "../Tools/Log.h"

namespace Sgl
{
	Surface::Surface(std::string_view path)
		: _surface(IMG_Load(path.data()))
	{
		if(_surface == nullptr)
		{
			Log::PrintSDLError();
		}
	}

	Surface::Surface(Surface&& other) noexcept
		: _surface(std::exchange(other._surface, nullptr))
	{}

	Surface::~Surface() noexcept
	{
		if(_surface)
		{
			SDL_FreeSurface(_surface);
		}
	}

	std::pair<size_t, size_t> Surface::Size() const
	{
		return std::pair{ _surface->w, _surface->h };
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		_surface = std::exchange(other._surface, nullptr);
		return *this;
	}
}
