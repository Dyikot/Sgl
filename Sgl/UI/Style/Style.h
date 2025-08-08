#pragma once

#include <memory>
#include <vector>
#include "IStyle.h"
#include "Setter.h"
#include "../../Base/SmartPointers.h"

namespace Sgl
{
    template<typename T>
    class Style: public IStyle
    {
    private:
        std::vector<Unique<ISetter<T>>> _setters;
    public:
        Style() = default;
        Style(const Style&) = default;
        Style(Style&&) = default;
        ~Style() = default;

        auto begin() { return _setters.begin(); }
        auto end() { return _setters.end(); }

        auto begin() const { return _setters.begin(); }
        auto end() const { return _setters.end(); }

        template<typename TOwner, typename TValue, typename TInput = T>
        Style<T>& With(BindableProperty<TOwner, TValue, TInput>& property, 
                       const std::remove_reference_t<TInput>& value)
        {
            _setters.push_back(NewUnique<Setter<T, TOwner, TValue, TInput>>(property, value));
            return *this;
        }

        void Apply(StyleableElement& target) override
        {
            T& targetElement = static_cast<T&>(target);
            for(const auto& setter : _setters)
            {
                setter->Apply(targetElement);
            }
        }
    };    
}