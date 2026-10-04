#include "Cursor.h"

#include <array>
#include <unordered_map>
#include <SDL3/SDL_mouse.h>

#include "../Base/Logging.h"
#include "../Render/Surface.h"
#include "../Base/Tools/StringUtils.h"

namespace Sgl
{
    static SDL_Cursor* CreateSystemCursor(Cursors systemCursor)
    {
        static std::array<SDL_Cursor*, 12> systems {};
        
        auto index = static_cast<size_t>(systemCursor);
        auto& cursor = systems[index];

        if(cursor == nullptr)
        {
            cursor = SDL_CreateSystemCursor(SDL_SystemCursor(index));
        }

        return cursor;
    }

    static SDL_Cursor* CreateCustomCursor(std::string_view filePath, Point hotSpot)
    {
        static std::unordered_map<std::string, SDL_Cursor*, StringHash, std::equal_to<>> customs;
        
        if(auto it = customs.find(filePath); it != customs.end())
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
            customs.emplace(filePath, cursor);
        }

        return cursor;
    }

    Cursor::Cursor(Cursors systemCursor) noexcept:
        _cursor(CreateSystemCursor(systemCursor))
    {}

    Cursor::Cursor(std::string_view filePath, Point hotSpot):
        _cursor(CreateCustomCursor(filePath, hotSpot))
    {}

    Cursor::Cursor(const Cursor& other):
        _cursor(other._cursor)
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(other._cursor)
    {}
}