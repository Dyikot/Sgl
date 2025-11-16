#include "Cursor.h"
#include <SDL3/SDL_log.h>
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

namespace Sgl
{
    struct CursorDeleter
    {
        void operator()(SDL_Cursor* cursor) const
        {
            SDL_DestroyCursor(cursor);
        }
    };

    static std::shared_ptr<SDL_Cursor> GetSystemCursor(SDL_SystemCursor cursor)
    {
        switch(cursor)
        {
            case Cursors::Arrow:
            {
                static std::shared_ptr<SDL_Cursor> arrow(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return arrow;
            }
            case Cursors::IBeam:
            {
                static std::shared_ptr<SDL_Cursor> ibeam(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return ibeam;
            }
            case Cursors::Wait:
            {
                static std::shared_ptr<SDL_Cursor> wait(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return wait;
            }
            case Cursors::Crosshair:
            {
                static std::shared_ptr<SDL_Cursor> crossHair(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return crossHair;
            }
            case Cursors::Progress:
            {
                static std::shared_ptr<SDL_Cursor> progress(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return progress;
            }
            case Cursors::ResizeNWSE:
            {
                static std::shared_ptr<SDL_Cursor> nwse(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return nwse;
            }
            case Cursors::ResizeNESW:
            {
                static std::shared_ptr<SDL_Cursor> nesw(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return nesw;
            }
            case Cursors::ResizeEW:
            {
                static std::shared_ptr<SDL_Cursor> ew(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return ew;
            }
            case Cursors::ResizeNS:
            {
                static std::shared_ptr<SDL_Cursor> ns(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return ns;
            }
            case Cursors::Move:
            {
                static std::shared_ptr<SDL_Cursor> move(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return move;
            }
            case Cursors::NotAllowed:
            {
                static std::shared_ptr<SDL_Cursor> nowAllowed(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return nowAllowed;
            }
            case Cursors::Pointer:
            {
                static std::shared_ptr<SDL_Cursor> pointer(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return pointer;
            }
            default:
                throw std::invalid_argument("Selected system cursor does exist");
        }
    }

    Cursor::Cursor(std::string_view path):
        _cursor(SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0), CursorDeleter())
    {
        if(_cursor == nullptr)
        {
            SDL_Log("Unable to create a cursor: %s", SDL_GetError());
        }
    }

    Cursor::Cursor(SDL_SystemCursor systemCursor) noexcept:
        _cursor(GetSystemCursor(systemCursor))
    {}

    Cursor::Cursor(const Cursor& other):
        _cursor(other._cursor)
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(std::exchange(other._cursor, nullptr))
    {}

    void Cursor::Set(const Cursor& cursor)
    {
        auto sdlCursor = cursor.GetSDLCursor();
        if(sdlCursor && sdlCursor != SDL_GetCursor())
        {
            SDL_SetCursor(sdlCursor);
        }
    }

    void Cursor::Show()
    {
        if(SDL_ShowCursor())
        {
            SDL_Log("Unable to show a cursor: %s", SDL_GetError());
        }
    }

    void Cursor::Hide()
    {
        if(SDL_HideCursor())
        {
            SDL_Log("Unable to hide a cursor: %s", SDL_GetError());
        }
    }

    bool Cursor::IsVisible()
    {
        return SDL_CursorVisible();
    }
}