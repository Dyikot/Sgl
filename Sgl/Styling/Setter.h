#pragma once

#include "../Data/StyleableProperty.h"

namespace Sgl
{
    class Styleable;

    /// <summary>
    /// Base class for all style setters.
    /// A setter applies a value to a property on a styleable element.
    /// </summary>
    class Setter
    {
    public:
        Setter(RestorableProperty& property): _property(property) {}
        virtual ~Setter() = default;

        /// <summary>
        /// Gets the property that this setter modifies.
        /// </summary>
        RestorableProperty& GetProperty() const { return _property; }

        /// <summary>
        /// Applies the setter's value to the specified target element.
        /// </summary>
        /// <param name="target"> - the target element.</param>
        /// <param name="valueSource"> - the source of the value (Style, Local, etc.).</param>
        virtual void Apply(Styleable& target, ValueSource valueSource) const = 0;
    private:
        RestorableProperty& _property;
    };

    /// <summary>
    /// A setter that applies a fixed value to a property.
    /// </summary>
    template<typename TOwner, typename TValue>
    class ValueSetter final : public Setter
    {
    private:
        using Value = std::remove_reference_t<TValue>;
    public:
        /// <summary>
        /// Initializes a new setter with the specified property and value.
        /// </summary>
        /// <param name="property"> - the property to set.</param>
        /// <param name="value"> - the value to apply.</param>
        ValueSetter(StyleableProperty<TOwner, TValue>& property, TValue value):
            Setter(property),
            _value(value)
        {}

        void Apply(Styleable& target, ValueSource valueSource) const
        {
            auto& property = static_cast<StyleableProperty<TOwner, TValue>&>(GetProperty());
            property.InvokeSetter(static_cast<TOwner&>(target), _value, valueSource);
        }
    private:
        Value _value;
    };

    /// <summary>
    /// A setter that resolves a value from a theme resource at runtime.
    /// Specializations are provided for specific property types.
    /// </summary>
    template<typename TOwner, typename TValue>
    class ResourceSetter;
}