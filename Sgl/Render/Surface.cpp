#include "Surface.h"

#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "../Base/Logging.h"

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
			Logging::LogError("Unable to load a surface: {}", SDL_GetError());
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
			Logging::LogError("Unable to create a surface: {}", SDL_GetError());
		}
	}

	Surface::Surface(Size size, SDL_PixelFormat format,
					 void* pixels, int pitch):
		_surface(SDL_CreateSurfaceFrom(size.Width, size.Height, format, pixels, pitch))
	{
		if(_surface == nullptr)
		{
			Logging::LogError("Unable to create a surface: {}", SDL_GetError());
		}
	}

	Surface::Surface(const Surface& other):
		_surface(other._surface)
	{
		if(_surface)
		{
			_surface->refcount++;
		}
	}

	Surface::Surface(Surface&& other) noexcept:
		_surface(other._surface)
	{
		other._surface = nullptr;
	}

	Surface::~Surface()
	{
		Release();
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

	uint32_t Surface::GetWidth() const
	{
		return static_cast<uint32_t>(_surface->w);
	}

	uint32_t Surface::GetHeight() const
	{
		return static_cast<uint32_t>(_surface->h);
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

	void Surface::Fill(const Rect* rect, Color color)
	{
		SDL_FillSurfaceRect(_surface, rect, color.ToRgba());
	}

	void Surface::FillRects(std::span<const Rect> rects, Color color)
	{
		SDL_FillSurfaceRects(_surface, rects.data(), rects.size(), color.ToRgba());
	}

	void Surface::Lock()
	{
		SDL_LockSurface(_surface);
	}

	void Surface::Unlock()
	{
		SDL_UnlockSurface(_surface);
	}

	void Surface::Flip(FlipMode flipMode)
	{
		SDL_FlipSurface(_surface, SDL_FlipMode(flipMode));
	}

	Surface Surface::Clone() const
	{
		auto surface = SDL_DuplicateSurface(_surface);

		if(surface == nullptr)
		{
			Logging::LogError("Unable to clone a surface: {}", SDL_GetError());
		}

		return Surface(surface);
	}

	Surface& Surface::operator=(std::nullptr_t)
	{
		Release();
		_surface = nullptr;
		return *this;
	}

	Surface& Surface::operator=(const Surface& other)
	{
		Release();
		_surface = other._surface;
		if(_surface)
		{
			_surface->refcount++;
		}

		return *this;
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		Release();
		_surface = other._surface;
		other._surface = nullptr;
		return *this;
	}

	void Surface::Release()
	{
		if(_surface)
		{
			SDL_DestroySurface(_surface);
		}
	}
}
