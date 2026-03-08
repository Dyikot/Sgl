#pragma once

#include <stdint.h>
#include "Property.h"

namespace Sgl
{
    class Layoutable;

    template<typename TValue>
    class LayoutProperty : public PropertyBase
    {
    public:
        using Owner = Layoutable;
        using Value = TValue;
        using Setter = void(*)(Owner&, Value);
        using Getter = Value(*)(Owner&);

        LayoutProperty(Setter setter, Getter getter):
            _setter(setter),
            _getter(getter)
        {}
        LayoutProperty(const LayoutProperty&) = delete;
        LayoutProperty(LayoutProperty&&) = delete;

        void InvokeSetter(Owner& owner, Value value)
        {
            _setter(owner, value);
        }

        Value InvokeGetter(Owner& owner) const
        {
            return _getter(owner);
        }
    private:
        Setter _setter;
        Getter _getter;
    };
}