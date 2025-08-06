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
        _cursor(),
        _creationArgs(std::move(path)),
        _hasTriedToCreate()
    {}

    Cursor::Cursor(SDL_SystemCursor systemCursor) noexcept:
        _cursor(),
        _creationArgs(systemCursor),
        _hasTriedToCreate()
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(std::exchange(other._cursor, nullptr)),
        _creationArgs(std::move(other._creationArgs)),
        _hasTriedToCreate(other._hasTriedToCreate)
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
        if(!_hasTriedToCreate)
        {
            _cursor = CreateCursor();
            _hasTriedToCreate = true;
            Log::PrintSDLErrorIf(_cursor == nullptr);
        }

        return _cursor;
    }

    bool operator==(const Cursor& left, const Cursor& right)
    {
        return left._creationArgs == right._creationArgs;
    }

    Cursor& Cursor::operator=(Cursor&& other) noexcept
    {
        _cursor = std::exchange(other._cursor, nullptr);
        _creationArgs = std::move(other._creationArgs);
        _hasTriedToCreate = other._hasTriedToCreate;
        return *this;
    }

    SDL_Cursor* Cursor::CreateCursor() const
    {
        return std::visit(CursorFactory(), _creationArgs);
    }
}