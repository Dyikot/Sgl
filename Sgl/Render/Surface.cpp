#include "Surface.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

namespace Sgl
{
	Surface::Surface(std::nullptr_t):
		_surface(nullptr)
	{}

	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()))
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to load a surface: %s", SDL_GetError());
		}
	}

	Surface::Surface(SDL_Surface* sdlSurface):
		_surface(sdlSurface)
	{}

	Surface::Surface(Size size, SDL_PixelFormat format):
		_surface(SDL_CreateSurface(size.Width, size.Height, format))
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to create a surface: %s", SDL_GetError());
		}
	}

	Surface::Surface(Size size, SDL_PixelFormat format,
					 void* pixels, size_t pitch):
		_surface(SDL_CreateSurfaceFrom(size.Width, size.Height, format, pixels, pitch))
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to create a surface: %s", SDL_GetError());
		}
	}

	Surface::Surface(const Surface& other):
		_surface(SDL_DuplicateSurface(other._surface))
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to copy a surface: %s", SDL_GetError());
		}
	}

	Surface::Surface(Surface&& other) noexcept:
		_surface(std::exchange(other._surface, nullptr))
	{}

	Surface::~Surface()
	{
		if(_surface != nullptr)
		{
			SDL_DestroySurface(_surface);
		}
	}

	void Surface::SetColor(Color value)
	{
		SDL_SetSurfaceColorMod(_surface, value.Red, value.Green, value.Blue);
		SDL_SetSurfaceAlphaMod(_surface, value.Alpha);
	}

	Color Surface::GetColor() const
	{
		Color color = Colors::Transparent;
		SDL_GetSurfaceColorMod(_surface, &color.Red, &color.Green, &color.Blue);
		SDL_GetSurfaceAlphaMod(_surface, &color.Alpha);
		return color;
	}

	Size Surface::GetSize() const
	{
		return Size(_surface->w, _surface->h);
	}

	void Surface::SetBlendMode(SDL_BlendMode value)
	{
		SDL_SetSurfaceBlendMode(_surface, value);
	}

	SDL_BlendMode Surface::GetBlendMode() const
	{
		SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
		SDL_GetSurfaceBlendMode(_surface, &blendMode);
		return blendMode;
	}

	void Surface::Lock()
	{
		SDL_LockSurface(_surface);
	}

	void Surface::Unlock()
	{
		SDL_UnlockSurface(_surface);
	}

	void Surface::Flip(SDL_FlipMode flipMode)
	{
		SDL_FlipSurface(_surface, flipMode);
	}

	Surface& Surface::operator=(const Surface& other)
	{
		if(_surface != nullptr)
		{
			SDL_DestroySurface(_surface);
		}

		_surface = SDL_DuplicateSurface(other._surface);

		if(_surface == nullptr)
		{
			SDL_Log("Unable to load a surface: %s", SDL_GetError());
		}

		return *this;
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		_surface = std::exchange(other._surface, _surface);
		return *this;
	}
}
