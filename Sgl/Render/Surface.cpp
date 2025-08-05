#include "Surface.h"
#include "../Base/Log.h"

namespace Sgl
{
	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()))
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(SDL_Surface* surface):
		_surface(surface)
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(const std::string& text, const Font& font):
		_surface(font.Antialiasing 
			? TTF_RenderText_Blended(font, text.c_str(), font.Color)
			: TTF_RenderText_Solid(font, text.c_str(), font.Color))
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

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
