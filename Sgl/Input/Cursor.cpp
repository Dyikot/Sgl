#include "Cursor.h"

#include <unordered_map>
#include <SDL3/SDL_mouse.h>
#include "../Base/Logging.h"
#include "../Render/Surface.h"
#include "../Base/Exceptions.h"
#include "../Base/Tools/StringUtils.h"

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

        SDL_Cursor* Create(std::string_view filePath, Point hotSpot)
        {
            if(auto it = _customCursors.find(filePath); it != _customCursors.end())
            {
                return it->second;
            }

            Surface surface(filePath);
            auto cursor = SDL_CreateColorCursor(surface, hotSpot.x, hotSpot.y);

            if(cursor == nullptr)
            {
                Logging::LogError("Unable to create a cursor: {}", SDL_GetError());
            }
            else
            {
                _customCursors.emplace(filePath, cursor);
            }

            return cursor;
        }
    private:
        SDL_Cursor* _systemCursors[12] {};
        std::unordered_map<std::string, SDL_Cursor*, StringHash, std::equal_to<>> _customCursors;
    };

    static CursorsPool cursorsPool;

    Cursor::Cursor(Cursors systemCursor) noexcept:
        _cursor(cursorsPool.Create(systemCursor))
    {}

    Cursor::Cursor(std::string_view filePath, Point hotSpot):
        _cursor(cursorsPool.Create(filePath, hotSpot))
    {}

    Cursor::Cursor(const Cursor& other):
        _cursor(other._cursor)
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(other._cursor)
    {}

    void SetCurrentCursor(Cursor cursor)
    {
        SDL_SetCursor(cursor);
    }

    void ShowCursor()
    {
        SDL_ShowCursor();
    }

    void HideCursor()
    {
        SDL_HideCursor();
    }

    bool IsCursorVisible()
    {
        return SDL_CursorVisible();
    }
}