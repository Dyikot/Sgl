#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Style.h"
#include "../../Base/SmartPointers.h"

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
            auto style = NewRef<Style<T>>();
            _items.emplace(key, style);
            return style;
        }

        const Ref<IStyle>* TryGet(const std::string& key) const;
        void Remove(const std::string& key);
        bool IsEmpty() const;

        StyleMap& operator=(const StyleMap& other);
        StyleMap& operator=(StyleMap&& other) noexcept;
    };

    class IStyleProvider
    {
    public:
        virtual ~IStyleProvider() = default;

        virtual StyleMap& GetStyles() = 0;
        virtual IStyleProvider* GetStylingParent() = 0;
    };
}