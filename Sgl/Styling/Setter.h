#pragma once

#include "../Data/SglProperty.h"

namespace Sgl
{
    class StyleableElement;

    class ISetter
    {
    public:
        virtual ~ISetter() = default;

        virtual void Apply(StyleableElement& target) const = 0;
    };

    template<typename TOwner, typename TValue>
    class Setter : public ISetter
    {
    private:
        SglProperty<TOwner, TValue>& _property;
        TValue _value;
    public:
        Setter(SglProperty<TOwner, TValue>& property, TValue value):
            _property(property),
            _value(value)
        {}

        void Apply(StyleableElement& target) const final
        {
            _property.InvokeSetter(static_cast<TOwner&>(target), _value);
        }
    };
}