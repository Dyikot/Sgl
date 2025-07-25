#pragma once

#include <memory>
#include <vector>
#include "IStyle.h"
#include "Setter.h"
#include "../../Base/SmartPointers.h"

namespace Sgl
{
    template<typename TTarget>
    class Style: public IStyle
    {
    private:
        std::vector<Unique<ISetter<TTarget>>> _setters;
    public:
        Style() = default;

        auto begin() { return _setters.begin(); }
        auto end() { return _setters.end(); }

        auto begin() const { return _setters.begin(); }
        auto end() const { return _setters.end(); }

        template<typename TProperty>
        Style<TTarget>& AddSetter(TProperty TTarget::* field, TProperty::InputType value)
        {
            using TField = TProperty TTarget::*;
            using TValue = TProperty::Type;

            _setters.push_back(CreateUnique<Setter<TTarget, TField, TValue>>(field, value));
            return *this;
        }

        void Apply(StyleableElement& target) override
        {
            TTarget& targetElement = static_cast<TTarget&>(target);
            for(const auto& setter : _setters)
            {
                setter->Apply(targetElement);
            }
        }
    };    
}