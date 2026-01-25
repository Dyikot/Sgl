#pragma once

#include "../Data/StyleableProperty.h"
#include "../Base/Media/ThemeVartiant.h"

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
        StyleableProperty<TOwner, TValue>& _property;
        TValue _value;
    public:
        Setter(StyleableProperty<TOwner, TValue>& property, TValue value):
            _property(property),
            _value(value)
        {}

        void Apply(StyleableElement& target) const final
        {
            _property.InvokeSetter(static_cast<TOwner&>(target), _value, ValueSource::Style);
        }
    };

    template<typename TOwner, typename TValue>
    class ThemeResourceSetter : public ISetter
    {
    private:
        using Value = std::decay_t<TValue>;

        StyleableProperty<TOwner, TValue>& _property;
        const ThemeResource<Value>& _resource;
    public:
        ThemeResourceSetter(StyleableProperty<TOwner, TValue>& property, 
                            const ThemeResource<Value>& resource):
            _property(property),
            _resource(resource)
        {}

        void Apply(StyleableElement& target) const final
        {
            TValue value = GetApplicationThemeMode() == ThemeMode::Light
                ? _resource.LightValue
                : _resource.DarkValue;

            _property.InvokeSetter(static_cast<TOwner&>(target), value, ValueSource::Style);
        }
    };
}