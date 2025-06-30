#include "Cursor.h"
#include "../Tools/Log.h"

namespace Sgl
{
    struct CursorFactory
    {
        SDL_Cursor* operator()(std::string_view path) const
        {
            return SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0);
        }

        SDL_Cursor* operator()(SDL_SystemCursor systemCursor) const
        {
            return SDL_CreateSystemCursor(systemCursor);
        }
    };

    void Cursor::Set(std::shared_ptr<Cursor> cursor)
    {
        auto sdlCursor = cursor->GetSDLCursor();
        if(sdlCursor != nullptr && SDL_GetCursor() != sdlCursor)
        {
            SDL_SetCursor(sdlCursor);
        }
    }

    Cursor::Cursor() noexcept:
        Cursor(SDL_SYSTEM_CURSOR_ARROW)
    {}

    Cursor::Cursor(std::string_view path):
        _cursor(nullptr),
        _creationArgs(path)
    {}

    Cursor::Cursor(SDL_SystemCursor systemCursor) noexcept:
        _cursor(nullptr),
        _creationArgs(systemCursor)
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(std::exchange(other._cursor, nullptr)),
        _creationArgs(std::move(other._creationArgs))
    {}

    Cursor::~Cursor() noexcept
    {
        if(_cursor)
        {
            SDL_FreeCursor(_cursor);
        }
    }

    SDL_Cursor* Cursor::GetSDLCursor() const
    {
        if(!_cursor)
        {
            _cursor = CreateCursor();
            Log::PrintSDLErrorIf(_cursor == nullptr);
        }

        return _cursor;
    }

    Cursor& Cursor::operator=(Cursor&& other) noexcept
    {
        _cursor = std::exchange(other._cursor, nullptr);
        _creationArgs = std::move(other._creationArgs);
        return *this;
    }

    SDL_Cursor* Cursor::CreateCursor() const
    {
        return std::visit(CursorFactory(), _creationArgs);
    }
}
