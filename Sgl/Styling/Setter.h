#pragma once

#include <string>
#include "../Base/Ref.h"
#include "../Data/StyleableProperty.h"

namespace Sgl
{
	class Styleable;

	//! @brief Base class for all style setters. A setter applies a value to a property on a styleable element.
	class ISetter : public RefCounted
	{
	public:
		//! @brief Gets target property
		//! @return Reference to property
		virtual PropertyBase& GetProperty() const = 0;

		//! @brief Applies the setter's value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value (Style, Local, etc.)
		virtual void Apply(Styleable& target, ValueSource valueSource) const = 0;
	};

	//! @brief A setter that applies a fixed value to a property
	template<typename TOwner, typename TValue>
	class Setter final : public ISetter
	{
	private:
		using Value = std::remove_reference_t<TValue>;
		using Property = StyleableProperty<TOwner, TValue>;
		using PropertyValue = Property::Value;
	public:
		//! @brief Initializes a new setter with the specified property and value
		//! @param property The property to set
		//! @param value The value to apply
		Setter(Property& property, PropertyValue value):
			_property(property),
			_value(value)
		{}

		//! @brief Gets target property
		//! @return Reference to property
		PropertyBase& GetProperty() const override
		{
			return _property;
		}

		//! @brief Applies the fixed value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			_property.InvokeSetter(static_cast<TOwner&>(target), _value, valueSource);
		}
	private:
		Property& _property;
		Value _value;
	};

	//! @brief A setter that resolves a value from a theme resource at runtime. 
	//! Specializations are provided for specific property types.
	template<typename TOwner, typename TValue>
	class ResourceSetter;

	template<typename TOwner, typename TValue>
	ResourceSetter(StyleableProperty<TOwner, TValue>&, std::string) -> ResourceSetter<TOwner, TValue>;
}