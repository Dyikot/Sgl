#include "Cursor.h"
#include "../Base/Log.h"

namespace Sgl
{
    struct CursorFactory
    {
        SDL_Cursor* operator()(const std::string& path) const
        {
            return SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0);
        }

        SDL_Cursor* operator()(SDL_SystemCursor systemCursor) const
        {
            return SDL_CreateSystemCursor(systemCursor);
        }
    };

    void Cursor::Set(const Cursor& cursor)
    {
        auto sdlCursor = cursor.GetSDLCursor();
        if(sdlCursor && sdlCursor != SDL_GetCursor())
        {
            SDL_SetCursor(sdlCursor);
        }
    }

    Cursor::Cursor() noexcept:
        Cursor(SDL_SYSTEM_CURSOR_ARROW)
    {}

    Cursor::Cursor(std::string path):
        _cursor(nullptr),
        _creationArgs(std::move(path))
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

    SDL_Cursor* Cursor::GetSDLCursor() const noexcept
    {
        if(!_cursor)
        {
            _cursor = CreateCursor();
            Log::PrintSDLErrorIf(_cursor == nullptr);
        }

        return _cursor;
    }

    bool operator==(const Cursor& left, const Cursor& right)
    {
        return left.GetSDLCursor() == right.GetSDLCursor();
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
