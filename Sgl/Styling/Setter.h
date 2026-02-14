#pragma once

#include "../Data/StyleableProperty.h"

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
    public:
        Setter(StyleableProperty<TOwner, TValue>& property, TValue value):
            _property(property),
            _value(value)
        {}

        void Apply(StyleableElement& target) const final
        {
            _property.InvokeSetter(static_cast<TOwner&>(target), _value, ValueSource::Style);
        }
    private:
        StyleableProperty<TOwner, TValue>& _property;
        TValue _value;
    };

    template<typename TOwner, typename TValue, typename TResources, typename TResource>
        requires std::constructible_from<TValue, TResource>
    class ResourceSetter : public ISetter
    {
    public:
        ResourceSetter(StyleableProperty<TOwner, TValue>& property, 
                       TResources& resources,
                       TResource TResources::* resource):
            _property(property),
            _resources(resources),
            _resource(resource)
        {}

        void Apply(StyleableElement& target) const final
        {
            _property.InvokeSetter(static_cast<TOwner&>(target),
                                   _resources.*_resource,
                                   ValueSource::Style);
        }
    private:
        StyleableProperty<TOwner, TValue>& _property;
        TResources& _resources;
        TResource TResources::* _resource;
    };
}