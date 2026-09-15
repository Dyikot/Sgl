#pragma once

#include "../Data/StyleableProperty.h"

namespace Sgl
{
	class Styleable;

	//! @brief Base class for all style setters. A setter applies a value to a property on a styleable element.
	class Setter
	{
	public:
		//! @brief Constructs a setter for the specified property
		//! @param property The property to modify
		Setter(StyleablePropertyBase& property): _property(property) {}

		//! @brief Default virtual destructor
		virtual ~Setter() = default;

		//! @brief Gets the property that this setter modifies
		//! @return A reference to the styleable property
		StyleablePropertyBase& GetProperty() const { return _property; }

		//! @brief Applies the setter's value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value (Style, Local, etc.)
		virtual void Apply(Styleable& target, ValueSource valueSource) const = 0;

	private:
		StyleablePropertyBase& _property;
	};

	//! @brief A setter that applies a fixed value to a property
	template<typename TOwner, typename TValue>
	class ValueSetter final: public Setter
	{
	private:
		using Value = std::remove_reference_t<TValue>;

	public:
		//! @brief Initializes a new setter with the specified property and value
		//! @param property The property to set
		//! @param value The value to apply
		ValueSetter(StyleableProperty<TOwner, TValue>& property, TValue value):
			Setter(property),
			_value(value)
		{}

		//! @brief Applies the fixed value to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			auto& property = static_cast<StyleableProperty<TOwner, TValue>&>(GetProperty());
			property.InvokeSetter(static_cast<TOwner&>(target), _value, valueSource);
		}

	private:
		Value _value;
	};

	//! @brief A setter that resolves a value from a theme resource at runtime. 
	//! Specializations are provided for specific property types.
	template<typename TOwner, typename TValue>
	class ResourceSetter;
}