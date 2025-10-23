#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Style.h"
#include "../Base/Ref.h"

namespace Sgl
{
    class StyleMap
    {
    private:
        std::unordered_map<std::string_view, Ref<IStyle>> _items;
    public:
        StyleMap() = default;
        StyleMap(const StyleMap& other);
        StyleMap(StyleMap&& other) noexcept;
        ~StyleMap() = default;

        auto begin() { return _items.begin(); }
        auto end() { return _items.end(); }

        auto begin() const { return _items.begin(); }
        auto end() const { return _items.end(); }

        template<typename T>
        Style<T>& Add(std::string_view key)
        {
            auto [it, _] = _items.emplace(key, New<Style<T>>());
            return it->second.GetValueAs<Style<T>>();
        }

        Ref<IStyle> TryGet(std::string_view key) const;
        void Remove(std::string_view key);
        bool IsEmpty() const;

        StyleMap& operator=(const StyleMap& other);
        StyleMap& operator=(StyleMap&& other) noexcept;
    };    
}