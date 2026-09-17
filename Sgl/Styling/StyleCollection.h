#pragma once

#include "Style.h"

namespace Sgl
{
    //! @brief A collection of styles that can be applied to UI elements
    class StyleCollection
    {
    public:
        StyleCollection() = default;
        StyleCollection(const StyleCollection&) = delete;
        StyleCollection(StyleCollection&&) noexcept = default;

        auto begin() const { return _items.begin(); }
        auto rbegin() const { return _items.rbegin(); }

        auto end() const { return _items.end(); }
        auto rend() const { return _items.rend(); }

        //! @brief Adds a new style with the specified selector to the collection
        //! @param selector The selector that determines which elements this style applies to
        //! @return A reference to the newly added style
        Style& Add(Selector selector)
        {
            return _items.emplace_back(std::move(selector));
        }

        //! @brief Gets the number of styles in the collection
        size_t Count() const noexcept
        {
            return _items.size();
        }

        //! @brief Determines whether the collection is empty
        bool IsEmpty() const noexcept
        {
            return _items.empty();
        }

        //! @brief Gets the style at the specified index (const version)
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