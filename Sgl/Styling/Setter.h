#pragma once

#include "../Data/StyleableProperty.h"

namespace Sgl
{
    class StyleableElement;

    class SetterBase
    {
    public:
        SetterBase(StyleablePropertyBase& property): _property(property) {}
        virtual ~SetterBase() = default;

        StyleablePropertyBase& GetProperty() const { return _property; }
        virtual void Apply(StyleableElement& target, ValueSource valueSource) const = 0;
    private:
        StyleablePropertyBase& _property;
    };

    template<typename TOwner, typename TValue>
    class Setter : public SetterBase
    {
    private:
        using Value = std::remove_reference_t<TValue>;
    public:
        Setter(StyleableProperty<TOwner, TValue>& property, TValue value):
            SetterBase(property),
            _value(value)
        {}

        void Apply(StyleableElement& target, ValueSource valueSource) const final
        {
            auto& property = static_cast<StyleableProperty<TOwner, TValue>&>(GetProperty());
            property.InvokeSetter(static_cast<TOwner&>(target), _value, valueSource);
        }
    private:
        Value _value;
    };

    template<typename TOwner, typename TValue>
    class ResourceSetter;
}