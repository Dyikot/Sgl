#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Style.h"

namespace Sgl
{
    class StyleMap
    {
    private:
        std::unordered_map<std::string_view, std::unique_ptr<IStyle>> _items;
    public:
        StyleMap() = default;
        StyleMap(const StyleMap&) = delete;
        StyleMap(StyleMap&& other) noexcept;
        ~StyleMap() = default;

        auto begin() { return _items.begin(); }
        auto begin() const { return _items.begin(); }

        auto end() { return _items.end(); }
        auto end() const { return _items.end(); }

        template<typename T>
        Style<T>& Add(std::string_view key)
        {
            auto [it, _] = _items.emplace(key, std::make_unique<Style<T>>());
            return static_cast<Style<T>&>(*it->second);
        }

        IStyle* TryGet(std::string_view key) const;
        void Remove(std::string_view key);
        bool IsEmpty() const;

        StyleMap& operator=(const StyleMap&) = delete;
        StyleMap& operator=(StyleMap&& other) noexcept;
    };    
}