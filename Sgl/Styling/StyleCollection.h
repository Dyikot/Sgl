#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Style.h"

namespace Sgl
{
    class StyleCollection
    {
    private:
        std::vector<Style> _items;
    public:
        StyleCollection() = default;
        StyleCollection(const StyleCollection&) = delete;
        StyleCollection(StyleCollection&&) noexcept = default;

        auto begin() { return _items.begin(); }
        auto begin() const { return _items.begin(); }

        auto end() { return _items.end(); }
        auto end() const { return _items.end(); }

        Style& Add(Selector selector)
        {
            return _items.emplace_back(std::move(selector));
        }

        bool IsEmpty() const 
        { 
            return _items.size() == 0; 
        }

        StyleCollection& operator=(const StyleCollection&) = delete;
        StyleCollection& operator=(StyleCollection&&) noexcept = default;
    };    
}