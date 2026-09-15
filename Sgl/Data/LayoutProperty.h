#pragma once

#include "Property.h"

namespace Sgl
{
    class Layoutable;

    //! @brief Represent a layout property that wraps a getter and setter method pair
    template<typename TValue>
    class LayoutProperty : public PropertyBase
    {
    public:
        using Owner = Layoutable;
        using Value = TValue;
        using Setter = void(*)(Owner&, Value);
        using Getter = Value(*)(Owner&);
    public:
        //! @brief Initializes a new instance with both getter and setter
        //! @param setter The setter method
        //! @param getter The getter method
        LayoutProperty(Setter setter, Getter getter):
            _setter(setter),
            _getter(getter)
        {}

        LayoutProperty(const LayoutProperty&) = delete;
        LayoutProperty(LayoutProperty&&) = delete;

        //! @brief Invokes the setter on the specified owner with the given value
        //! @param owner The owner object
        //! @param value The value to set
        void InvokeSetter(Owner& owner, Value value)
        {
            _setter(owner, value);
        }

        //! @brief Invokes the getter on the specified owner and returns the value
        //! @param Owner the owner object
        //! @return The property value
        Value InvokeGetter(Owner& owner) const
        {
            return _getter(owner);
        }

    private:
        Setter _setter;
        Getter _getter;
    };
}