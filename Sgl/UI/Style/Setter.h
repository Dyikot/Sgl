#pragma once

#include <concepts>
#include "../Binding/BindableProperty.h"

namespace Sgl
{
    class StyleableElement;

    template<std::derived_from<StyleableElement> TTarget>
    class ISetter
    {
    public:
        virtual ~ISetter() = default;

        virtual void Apply(TTarget& target) const = 0;
    };

    template<typename TTarget, typename TOwner, typename TValue>
    class Setter: public ISetter<TTarget>
    {
    public: 
        BindableProperty<TOwner, TValue>& Property;
        TValue Value;
    public:
        Setter(BindableProperty<TOwner, TValue>& property, TValue value):
            Property(property), Value(value)
        {}

        void Apply(TTarget& target) const
        {
            (target.*(Property.PropertySetter))(Value);
        }
    };
}