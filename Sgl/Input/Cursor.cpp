#include "Cursor.h"
#include "../Base/Log.h"

namespace Sgl
{
    struct CursorDeleter
    {
        void operator()(SDL_Cursor* cursor) const
        {
            SDL_FreeCursor(cursor);
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

    static std::shared_ptr<SDL_Cursor> GetSystemCursor(SDL_SystemCursor cursor)
    {
        switch(cursor)
        {
            case SDL_SYSTEM_CURSOR_ARROW:
            {
                static std::shared_ptr<SDL_Cursor> arrow(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return arrow;
            }
            case SDL_SYSTEM_CURSOR_IBEAM: 
            {
                static std::shared_ptr<SDL_Cursor> ibeam(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return ibeam;
            }
            case SDL_SYSTEM_CURSOR_WAIT:
            {
                static std::shared_ptr<SDL_Cursor> wait(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return wait;
            }
            case SDL_SYSTEM_CURSOR_CROSSHAIR:
            {
                static std::shared_ptr<SDL_Cursor> crossHair(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return crossHair;
            }
            case SDL_SYSTEM_CURSOR_WAITARROW:
            {
                static std::shared_ptr<SDL_Cursor> waitArrow(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return waitArrow;
            }
            case SDL_SYSTEM_CURSOR_SIZENWSE:
            {
                static std::shared_ptr<SDL_Cursor> sizeNWSE(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return sizeNWSE;
            }
            case SDL_SYSTEM_CURSOR_SIZENESW:
            {
                static std::shared_ptr<SDL_Cursor> sizeNESW(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return sizeNESW;
            }
            case SDL_SYSTEM_CURSOR_SIZEWE:
            {
                static std::shared_ptr<SDL_Cursor> sizeWE(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return sizeWE;
            }
            case SDL_SYSTEM_CURSOR_SIZENS:
            {
                static std::shared_ptr<SDL_Cursor> sizeNS(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return sizeNS;
            }
            case SDL_SYSTEM_CURSOR_SIZEALL:
            {
                static std::shared_ptr<SDL_Cursor> sizeALL(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return sizeALL;
            }
            case SDL_SYSTEM_CURSOR_NO:
            {
                static std::shared_ptr<SDL_Cursor> sizeNO(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return sizeNO;
            }
            case SDL_SYSTEM_CURSOR_HAND: 
            {
                static std::shared_ptr<SDL_Cursor> hand(SDL_CreateSystemCursor(cursor), CursorDeleter());
                return hand;
            }
            default:
                throw std::invalid_argument("Selected system cursor does exist");
        }
    }

    Cursor::Cursor(std::string_view path):
        _cursor(SDL_CreateColorCursor(IMG_Load(path.data()), 0, 0), CursorDeleter())
    {
        Log::PrintSDLErrorIf(_cursor == nullptr);
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
}