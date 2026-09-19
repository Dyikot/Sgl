#pragma once

#include "ISavedValue.h"

namespace Sgl
{
	class Styleable;

	//! @brief Base class for all style setters. A setter applies a value to a property on a styleable element.
	class Setter
	{
	public:
		virtual ~Setter() = default;

		//! @brief Applies the setter's value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value (Style, Local, etc.)
		virtual void Apply(Styleable& target, ValueSource valueSource) const = 0;

		//! @brief Save the current property value of the specified target element
		//! @param target The target element
		//! @return Saved value
		virtual ISavedValue* Save(Styleable& target) const = 0;
	};

	//! @brief A setter that applies a fixed value to a property
	template<typename TOwner, typename TValue>
	class ValueSetter final : public Setter
	{
	private:
		using Value = std::remove_reference_t<TValue>;
		using Property = StyleableProperty<TOwner, TValue>;
	public:
		//! @brief Initializes a new setter with the specified property and value
		//! @param property The property to set
		//! @param value The value to apply
		ValueSetter(Property& property, TValue value):
			_property(property),
			_value(value)
		{}

		//! @brief Applies the fixed value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			_property.InvokeSetter(static_cast<TOwner&>(target), _value, valueSource);
		}

		//! @brief Save current property of specified target element
		//! @param target The target element
		//! @return Saved value
		ISavedValue* Save(Styleable& target) const
		{
			auto& owner = static_cast<TOwner&>(target);
			return new SavedPropertyValue<TOwner, TValue>(_property, owner);
		}
	private:
		Property& _property;
		Value _value;
	};

	//! @brief A setter that resolves a value from a theme resource at runtime. 
	//! Specializations are provided for specific property types.
	template<typename TOwner, typename TValue>
	class ResourceSetter;
}