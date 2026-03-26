#pragma once

#include "Style.h"

namespace Sgl
{
    /// <summary>
    /// A collection of styles that can be applied to UI elements.
    /// Styles are stored in order and applied sequentially.
    /// </summary>
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

        /// <summary>
        /// Adds a new style with the specified selector to the collection.
        /// </summary>
        /// <param name="selector"> - the selector that determines which elements this style applies to.</param>
        /// <returns>A reference to the newly added style.</returns>
        Style& Add(Selector selector)
        {
            return _items.emplace_back(std::move(selector));
        }

        /// <summary>
        /// Adds a new style with a selector and target projection to the collection.
        /// </summary>
        /// <param name="selector"> - the selector that determines which elements this style applies to.</param>
        /// <param name="projection"> - the projection that specifies which part of the element to style.</param>
        /// <returns>A reference to the newly added style.</returns>
        Style& Add(Selector selector, TargetProjection projection)
        {
            return _items.emplace_back(std::move(selector), projection);
        }

        /// <summary>
        /// Gets the number of styles in the collection.
        /// </summary>
        size_t Count() const noexcept
        {
            return _items.size();
        }

        /// <summary>
        /// Determines whether the collection is empty.
        /// </summary>
        bool IsEmpty() const noexcept
        {
            return _items.empty();
        }

        /// <summary>
        /// Gets the style at the specified index.
        /// </summary>
        Style& operator[](size_t index)
        {
            return _items[index];
        }

        /// <summary>
        /// Gets the style at the specified index (const version).
        /// </summary>
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