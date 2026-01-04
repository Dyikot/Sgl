#include "Cursor.h"

#include <stdexcept>
#include <SDL3_image/SDL_image.h>
#include "../Base/Logger.h"
#include "../Render/Surface.h"

namespace Sgl
{
    struct Cursor::CursorImpl
    {
        SDL_Cursor* SdlCursor = nullptr;
        int References = 0;
    };

    static const Cursor& GetSystemCursor(SDL_SystemCursor cursor)
    {
        switch(cursor)
        {
            case Cursors::Arrow:
            {
                static Cursor arrow(SDL_CreateSystemCursor(cursor));
                return arrow;
            }
            case Cursors::IBeam:
            {
                static Cursor ibeam(SDL_CreateSystemCursor(cursor));
                return ibeam;
            }
            case Cursors::Wait:
            {
                static Cursor wait(SDL_CreateSystemCursor(cursor));
                return wait;
            }
            case Cursors::Crosshair:
            {
                static Cursor crossHair(SDL_CreateSystemCursor(cursor));
                return crossHair;
            }
            case Cursors::Progress:
            {
                static Cursor progress(SDL_CreateSystemCursor(cursor));
                return progress;
            }
            case Cursors::ResizeNWSE:
            {
                static Cursor nwse(SDL_CreateSystemCursor(cursor));
                return nwse;
            }
            case Cursors::ResizeNESW:
            {
                static Cursor nesw(SDL_CreateSystemCursor(cursor));
                return nesw;
            }
            case Cursors::ResizeEW:
            {
                static Cursor ew(SDL_CreateSystemCursor(cursor));
                return ew;
            }
            case Cursors::ResizeNS:
            {
                static Cursor ns(SDL_CreateSystemCursor(cursor));
                return ns;
            }
            case Cursors::Move:
            {
                static Cursor move(SDL_CreateSystemCursor(cursor));
                return move;
            }
            case Cursors::NotAllowed:
            {
                static Cursor nowAllowed(SDL_CreateSystemCursor(cursor));
                return nowAllowed;
            }
            case Cursors::Pointer:
            {
                static Cursor pointer(SDL_CreateSystemCursor(cursor));
                return pointer;
            }
            default:
                throw std::invalid_argument("Selected system cursor does exist");
        }
    }

    Cursor::Cursor(std::string_view path)
    {
        Surface surface(path);
        _cursor = new CursorImpl(SDL_CreateColorCursor(surface.GetSDLSurface(), 0, 0), 1);

        if(_cursor == nullptr)
        {
            Logger::LogError("Unable to create a cursor: {}", SDL_GetError());
        }
    }

    Cursor::Cursor(SDL_Cursor* sdlCursor):
        _cursor(new CursorImpl(sdlCursor, 1))
    {}

    Cursor::Cursor(SDL_SystemCursor systemCursor) noexcept:
        Cursor(GetSystemCursor(systemCursor))
    {}

    Cursor::Cursor(const Cursor& other)
    {
        CopyFrom(other);
    }

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(other._cursor)
    {
        other._cursor = nullptr;
    }

    Cursor::~Cursor()
    {
        Destroy();
    }

    void Cursor::Set(const Cursor& cursor)
    {
        auto sdlCursor = cursor._cursor->SdlCursor;
        if(sdlCursor != nullptr && sdlCursor != SDL_GetCursor())
        {
            SDL_SetCursor(sdlCursor);
        }
    }

    void Cursor::Show()
    {
        if(SDL_ShowCursor())
        {
            Logger::LogWarning("Unable to show a cursor: {}", SDL_GetError());
        }
    }

    void Cursor::Hide()
    {
        if(SDL_HideCursor())
        {
            Logger::LogWarning("Unable to hide a cursor: {}", SDL_GetError());
        }
    }

    bool Cursor::IsVisible()
    {
        return SDL_CursorVisible();
    }

    Cursor& Cursor::operator=(std::nullptr_t)
    {
        Destroy();
        return *this;
    }

    Cursor& Cursor::operator=(const Cursor& other)
    {
        Destroy();
        CopyFrom(other);
        return *this;
    }

    Cursor& Cursor::operator=(Cursor&& other) noexcept
    {
        Destroy();
        _cursor = other._cursor;
        other._cursor = nullptr;
        return *this;
    }

    void Cursor::CopyFrom(const Cursor& other)
    {
        _cursor = other._cursor;

        if(_cursor != nullptr)
        {
            _cursor->References++;
        }
    }

    void Cursor::Destroy()
    {
        if(_cursor != nullptr && --(_cursor->References) == 0)
        {
            if(auto sdlCursor = _cursor->SdlCursor)
            {
                SDL_DestroyCursor(sdlCursor);
            }
            
            delete _cursor;
            _cursor = nullptr;
        }
    }
}