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
        std::unordered_map<std::string, std::shared_ptr<IStyle>> _items;
    public:
        StyleMap() = default;
        StyleMap(const StyleMap& other);
        StyleMap(StyleMap&& other) noexcept;

        auto begin() { return _items.begin(); }
        auto end() { return _items.end(); }

        auto begin() const { return _items.begin(); }
        auto end() const { return _items.end(); }

        template<typename T>
        Style<T>& Add(std::string key)
        {
            auto style = std::make_shared<Style<T>>();
            _items.emplace(std::move(key), style);
            return *style.get();
        }

        std::shared_ptr<IStyle> TryGet(const std::string& key) const;
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