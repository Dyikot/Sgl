#pragma once

#include "Style.h"

namespace Sgl
{
    class StyleCollection
    {
    public:
        StyleCollection() = default;
        StyleCollection(const StyleCollection&) = delete;
        StyleCollection(StyleCollection&&) noexcept = default;

        auto begin() { return _items.begin(); }
        auto begin() const { return _items.begin(); }

        auto end() { return _items.end(); }
        auto end() const { return _items.end(); }

        auto rbegin() { return _items.rbegin(); }
        auto rbegin() const { return _items.rbegin(); }

        auto rend() { return _items.rend(); }
        auto rend() const { return _items.rend(); }

        Style& Add(Selector selector)
        {
            return _items.emplace_back(std::move(selector));
        }

        Style& Add(Selector selector, TargetProjection projection)
        {
            return _items.emplace_back(std::move(selector), projection);
        }

        size_t Count() const noexcept
        {
            return _items.size();
        }

        bool IsEmpty() const noexcept
        { 
            return _items.empty(); 
        }

        Style& operator[](size_t index)
        {
            return _items[index];
        }

        const Style& operator[](size_t index) const
        {
            return _items[index];
        }

        StyleCollection& operator=(const StyleCollection&) = delete;
        StyleCollection& operator=(StyleCollection&&) noexcept = default;
    private:
        std::vector<Style> _items;
    };    
}