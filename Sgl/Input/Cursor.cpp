#include "Cursor.h"

#include <stdexcept>
#include <unordered_map>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_mouse.h>
#include "../Base/Logging.h"
#include "../Render/Surface.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
    class CursorsPool
    {
    public:
        CursorsPool() = default;

        ~CursorsPool()
        {
            for(auto cursor : _systemCursors)
            {
                SDL_DestroyCursor(cursor);
            }

            for(auto& [_, cursor] : _customCursors)
            {
                SDL_DestroyCursor(cursor);
            }
        }

        SDL_Cursor* Create(Cursors systemCursor)
        {
            size_t id = static_cast<size_t>(systemCursor);
            auto& cursor = _systemCursors[id];

            if(cursor == nullptr)
            {
                cursor = SDL_CreateSystemCursor(SDL_SystemCursor(id));
            }

            return cursor;
        }

        SDL_Cursor* Create(std::string_view path)
        {
            if(auto it = _customCursors.find(path); it != _customCursors.end())
            {
                return it->second;
            }

            Surface surface(path);
            auto cursor = SDL_CreateColorCursor(surface.GetSDLSurface(), 0, 0);

            if(cursor == nullptr)
            {
                Logging::LogError("Unable to create a cursor: {}", SDL_GetError());
            }
            else
            {
                _customCursors.emplace(path, cursor);
            }

            return cursor;
        }
    private:
        SDL_Cursor* _systemCursors[12] {};
        std::unordered_map<std::string_view, SDL_Cursor*> _customCursors;
    };

    CursorsPool cursorsPool;

    Cursor::Cursor(Cursors systemCursor) noexcept:
        _cursor(cursorsPool.Create(systemCursor))
    {}

    Cursor::Cursor(std::string_view path):
        _cursor(cursorsPool.Create(path))
    {}

    Cursor::Cursor(const Cursor& other):
        _cursor(other._cursor)
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(other._cursor)
    {}

    void Cursor::Set(const Cursor& cursor)
    {
        if(cursor && cursor != SDL_GetCursor())
        {
            SDL_SetCursor(cursor);
        }
    }

    void Cursor::Show()
    {
        if(SDL_ShowCursor())
        {
            Logging::LogWarning("Unable to show a cursor: {}", SDL_GetError());
        }
    }

    void Cursor::Hide()
    {
        if(SDL_HideCursor())
        {
            Logging::LogWarning("Unable to hide a cursor: {}", SDL_GetError());
        }
    }

    bool Cursor::IsVisible()
    {
        return SDL_CursorVisible();
    }
}