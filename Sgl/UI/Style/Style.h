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

        auto begin() { return _setters.begin(); }
        auto end() { return _setters.end(); }

        auto begin() const { return _setters.begin(); }
        auto end() const { return _setters.end(); }

        template<typename TProperty>
        Style<T>& With(TProperty& property, TProperty::InputType value)
        {
            using TTarget = T;
            using TOwner = TProperty::OwnerType;
            using TValue = TProperty::Type;
            using TInput = TProperty::InputType;

            _setters.push_back(NewUnique<Setter<TTarget, TOwner, TValue, TInput>>(property, value));
            return *this;
        }

        Style<T>& With(Unique<ISetter<T>> setter)
        {
            _setters.push_back(std::move(setter));
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