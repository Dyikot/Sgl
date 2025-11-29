#pragma once

#include <memory>
#include <vector>

#include "../Base/Delegate.h"
#include "../Data/ObservableProperty.h"

namespace Sgl
{
    class StyleableElement;

    class IStyle
    {
    public:
        virtual ~IStyle() = default;

        virtual void Apply(StyleableElement& target) = 0;
    };

    template<std::derived_from<StyleableElement> T>
    class Style : public IStyle
    {
    private:
        std::vector<Action<T&>> _setters;
    public:
        Style() = default;
        Style(const Style&) = default;
        Style(Style&&) = default;
        ~Style() = default;

        auto begin() { return _setters.begin(); }
        auto end() { return _setters.end(); }

        auto begin() const { return _setters.begin(); }
        auto end() const { return _setters.end(); }

        template<typename TOwner, typename TValue>
        Style<T>& With(ObservableProperty<TOwner, TValue>& property, 
                       ObservableProperty<TOwner, TValue>::Value value)
        {
            _setters.emplace_back([&property, value](T& target)
            {
                property.Set(target, value);
            });

            return *this;
        }

        void Apply(StyleableElement& target) override
        {
            for(const auto& setter : _setters)
            {
                setter(static_cast<T&>(target));
            }
        }
    };    
}