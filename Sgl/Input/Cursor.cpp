#include "Cursor.h"

#include <array>
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
        ~CursorsPool()
        {
            for(auto cursor : _systems)
            {
                SDL_DestroyCursor(cursor);
            }

            for(auto& [_, cursor] : _customs)
            {
                SDL_DestroyCursor(cursor);
            }
        }

        static CursorsPool& Instance()
        {
            static CursorsPool pool;
            return pool;
        }

        SDL_Cursor* Get(Cursors systemCursor)
        {
            size_t id = static_cast<size_t>(systemCursor);
            auto& cursor = _systems[id];

            if(cursor == nullptr)
            {
                cursor = SDL_CreateSystemCursor(SDL_SystemCursor(id));
            }

            return cursor;
        }

        SDL_Cursor* Get(std::string_view filePath, Point hotSpot)
        {
            if(auto it = _customs.find(filePath); it != _customs.end())
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
                _customs.emplace(filePath, cursor);
            }

            return cursor;
        }

    private:
        CursorsPool() = default;

    private:
        std::array<SDL_Cursor*, 12> _systems {};
        std::unordered_map<std::string, SDL_Cursor*, StringHash, std::equal_to<>> _customs;
    };

    Cursor::Cursor(Cursors systemCursor) noexcept:
        _cursor(CursorsPool::Instance().Get(systemCursor))
    {}

    Cursor::Cursor(std::string_view filePath, Point hotSpot):
        _cursor(CursorsPool::Instance().Get(filePath, hotSpot))
    {}

    Cursor::Cursor(const Cursor& other):
        _cursor(other._cursor)
    {}

    Cursor::Cursor(Cursor&& other) noexcept:
        _cursor(other._cursor)
    {}
}