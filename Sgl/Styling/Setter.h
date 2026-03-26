#pragma once

#include "../Data/StyleableProperty.h"

namespace Sgl
{
    class StyleableElement;

    /// <summary>
    /// Base class for all style setters.
    /// A setter applies a value to a property on a styleable element.
    /// </summary>
    class SetterBase
    {
    public:
        SetterBase(StyleablePropertyBase& property): _property(property) {}
        virtual ~SetterBase() = default;

        /// <summary>
        /// Gets the property that this setter modifies.
        /// </summary>
        StyleablePropertyBase& GetProperty() const { return _property; }

        /// <summary>
        /// Applies the setter's value to the specified target element.
        /// </summary>
        /// <param name="target"> - the target element.</param>
        /// <param name="valueSource"> - the source of the value (Style, Local, etc.).</param>
        virtual void Apply(StyleableElement& target, ValueSource valueSource) const = 0;
    private:
        StyleablePropertyBase& _property;
    };

    /// <summary>
    /// A setter that applies a fixed value to a property.
    /// </summary>
    template<typename TOwner, typename TValue>
    class Setter : public SetterBase
    {
    private:
        using Value = std::remove_reference_t<TValue>;
    public:
        /// <summary>
        /// Initializes a new setter with the specified property and value.
        /// </summary>
        /// <param name="property"> - the property to set.</param>
        /// <param name="value"> - the value to apply.</param>
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

    /// <summary>
    /// A setter that resolves a value from a theme resource at runtime.
    /// Specializations are provided for specific property types.
    /// </summary>
    template<typename TOwner, typename TValue>
    class ResourceSetter;
}