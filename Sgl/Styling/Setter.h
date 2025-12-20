#pragma once

#include "../Data/ObservableProperty.h"

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
        ObservableProperty<TOwner, TValue>& _property;
        TValue _value;
    public:
        Setter(ObservableProperty<TOwner, TValue>& property, TValue value):
            _property(property),
            _value(value)
        {}

        void Apply(StyleableElement& target) const final
        {
            _property.Set(static_cast<TOwner&>(target), _value);
        }
    };
}