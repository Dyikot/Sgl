#include "Surface.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

namespace Sgl
{
	struct SurfaceDeleter
	{
		void operator()(SDL_Surface* surface) const
		{
			SDL_DestroySurface(surface);
		}
	};

	Surface::Surface(std::nullptr_t):
		_surface(nullptr)
	{}

	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()), SurfaceDeleter())
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to load a surface: %s", SDL_GetError());
		}
	}

	Surface::Surface(SDL_Surface* sdlSurface):
		_surface(sdlSurface, SurfaceDeleter())
	{}

	Surface::Surface(Size size, SDL_PixelFormat format):
		_surface(SDL_CreateSurface(size.Width, size.Height, format), SurfaceDeleter())
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to create a surface: %s", SDL_GetError());
		}
	}

	Surface::Surface(Size size, SDL_PixelFormat format,
					 void* pixels, size_t pitch):
		_surface(SDL_CreateSurfaceFrom(size.Width, size.Height, format, pixels, pitch), SurfaceDeleter())
	{
		if(_surface == nullptr)
		{
			SDL_Log("Unable to create a surface: %s", SDL_GetError());
		}
	}

	void Surface::SetColor(Color value)
	{
		SDL_SetSurfaceColorMod(_surface.get(), value.Red, value.Green, value.Blue);
		SDL_SetSurfaceAlphaMod(_surface.get(), value.Alpha);
	}

	Color Surface::GetColor() const
	{
		Color color = Colors::Transparent;
		SDL_GetSurfaceColorMod(_surface.get(), &color.Red, &color.Green, &color.Blue);
		SDL_GetSurfaceAlphaMod(_surface.get(), &color.Alpha);
		return color;
	}

	Size Surface::GetSize() const
	{
		return Size(_surface->w, _surface->h);
	}

	void Surface::SetBlendMode(SDL_BlendMode value)
	{
		SDL_SetSurfaceBlendMode(_surface.get(), value);
	}

	SDL_BlendMode Surface::GetBlendMode() const
	{
		SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
		SDL_GetSurfaceBlendMode(_surface.get(), &blendMode);
		return blendMode;
	}

	void Surface::Lock()
	{
		SDL_LockSurface(_surface.get());
	}

	void Surface::Unlock()
	{
		SDL_UnlockSurface(_surface.get());
	}

	void Surface::Flip(SDL_FlipMode flipMode)
	{
		SDL_FlipSurface(_surface.get(), flipMode);
	}

	Surface Surface::Clone() const
	{
		return Surface(SDL_DuplicateSurface(_surface.get()));
	}
}
