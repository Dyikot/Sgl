#include "Surface.h"
#include "../Base/Log.h"

namespace Sgl
{
	struct SurfaceDeleter
	{
		void operator()(SDL_Surface* surface) const
		{
			SDL_FreeSurface(surface);
		}
	};

	Surface::Surface(std::nullptr_t)
	{}

	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()), SurfaceDeleter())
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(uint32_t flags, Size size, int depth, uint32_t format):
		_surface(SDL_CreateRGBSurfaceWithFormat(
			flags, 
			size.Width, 
			size.Height, 
			depth, 
			format),
			SurfaceDeleter())
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(void* pixels, Size size, int depth, int pitch, uint32_t format):
		_surface(SDL_CreateRGBSurfaceWithFormatFrom(
			pixels, 
			size.Width, 
			size.Height, 
			depth,
			pitch,
			format),
			SurfaceDeleter())
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(uint32_t flags, Size size, int depth, 
					 uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask):
		_surface(SDL_CreateRGBSurface(
			flags, 
			size.Width, 
			size.Height, 
			depth,
			rmask, gmask, bmask, amask),
			SurfaceDeleter())
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(void* pixels, Size size, int depth, int pitch,
					 uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask):
		_surface(SDL_CreateRGBSurfaceFrom(
			pixels,
			size.Width,
			size.Height,
			depth,
			pitch,
			rmask, gmask, bmask, amask),
			SurfaceDeleter())
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
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
}
