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
        Style<T>& With(TProperty T::* field, TProperty::InputType value)
        {
            _setters.push_back(NewUnique<Setter<T, TProperty>>(field, value));
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