#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "../Data/Object.h"
#include "Setters.h"

using std::shared_ptr;

namespace Sgl
{
    struct Selector
    {
        std::string Class;
        std::string PseudoClass;
    };

    class IStyle
    {
    public:
        virtual ~IStyle() = default;

        virtual void ApplyTo(object target) = 0;
        virtual const Selector& GetSelector() const = 0;
    };

    template<typename T>
    class Style: public IStyle
    {
    private:
        Selector _selector;
    public:
        using Setter = SettersCollection<T>::Setter;

        SettersCollection<T> Setters;
    public:
        explicit Style(Selector selector): 
            _selector(std::move(selector))
        {}

        static shared_ptr<Style<T>> New(Selector selector)
        {
            return std::make_shared<Style<T>>(std::move(selector));
        }

        const Selector& GetSelector() const override 
        { 
            return _selector; 
        }

        void ApplyTo(object target) override
        {
            for(const Setter& setter : Setters)
            {
                setter(target.As<T>());
            }
        }
    };

    class StyleCollection
    {
    private:
        std::unordered_map<std::string, shared_ptr<IStyle>> _items;
    public:
        StyleCollection() = default;

        StyleCollection(const StyleCollection& other):
            _items(other._items)
        {}

        StyleCollection(StyleCollection&& other) noexcept:
            _items(std::move(other._items))
        {}

        auto begin() const { return _items.begin(); }
        auto end() const { return _items.end(); }

        void Add(shared_ptr<IStyle> style);
        shared_ptr<IStyle> TryFind(const std::string& className);
    };
}