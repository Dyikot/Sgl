#pragma once
#include <SDL/SDL_image.h>
#include <string_view>

class Icon
{
private:
	SDL_Surface* _surface = nullptr;
public:
	Icon(std::string_view path):
		_surface(IMG_Load(path.data()))
	{}
	Icon(Icon&& icon) noexcept:
		_surface(icon._surface)
	{
		icon._surface = nullptr;
	}
	~Icon() noexcept { SDL_FreeSurface(_surface); }

	operator SDL_Surface* () const { return _surface; }
	Icon& operator=(Icon&& icon) noexcept
	{
		_surface = icon._surface;
		icon._surface = nullptr;
		return *this;
	}
};