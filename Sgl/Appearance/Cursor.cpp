#include "Cursor.h"
#include "SDL/SDL_image.h"

namespace Sgl
{
	Cursor::Cursor() noexcept:
		Cursor(SDL_SYSTEM_CURSOR_ARROW)
	{}

	Cursor::Cursor(std::string_view path):
		_cursor(SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0))
	{}

	Cursor::Cursor(SDL_SystemCursor id) noexcept: 
		_cursor(SDL_CreateSystemCursor(id))
	{}

	Cursor::~Cursor() noexcept
	{
		SDL_FreeCursor(_cursor);
	}
}
