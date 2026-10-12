#pragma once

#include "../Base/Ref.h"
#include "../Data/StyleableProperty.h"

namespace Sgl
{
	class Styleable;

	//! @brief Base class for all style setters. A setter applies a value to a property on a styleable element.
	class SetterBase : public RefCounted
	{
	public:
		SetterBase(PropertyBase& property):
			_property(property)
		{}

		//! @brief Gets target property
		//! @return Reference to property
		PropertyBase& GetProperty() const { return _property; }

		//! @brief Applies the setter's value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value (Style, Local, etc.)
		virtual void Apply(Styleable& target, ValueSource valueSource) const = 0;

	protected:
		template<typename TOwner, typename TValue>
		void SetPropertyValue(Styleable& target, ValueSource valueSource, TValue value) const
		{
			auto& owner = static_cast<TOwner&>(target);
			auto& property = static_cast<StyleableProperty<TOwner, TValue>&>(_property);
			property.InvokeSetter(owner, value, valueSource);
		}

	private:
		PropertyBase& _property;
	};

	//! @brief A setter that applies a fixed value to a property
	template<typename TOwner, typename TValue>
	class Setter final : public SetterBase
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
			SetterBase(property),
			_value(value)
		{}

		//! @brief Applies the fixed value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			SetPropertyValue<TOwner, TValue>(target, valueSource, _value);
		}
	private:
		Value _value;
	};
}