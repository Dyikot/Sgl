#pragma once

#include <stdint.h>
#include "Property.h"

namespace Sgl
{
	//! @brief Determines the priority when setting the value
	enum class ValueSource : uint8_t
	{
		Default,
		Inheritance,
		Style,
		Local,
		PseudoClass
	};

	//! @brief Represents a styleable property that wraps a getter and setter method pair
	template<typename TOwner, typename TValue>
	class StyleableProperty : public PropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using Setter = void(Owner::*)(Value, ValueSource);
		using Getter = TValue(Owner::*)() const;
	public:
		//! @brief Initializes a new instance with both getter and setter
		//! @param setter The setter method
		//! @param getter The getter method
		StyleableProperty(Setter setter, Getter getter):
			_setter(setter), 
			_getter(getter)
		{}

		StyleableProperty(const StyleableProperty&) = delete;
		StyleableProperty(StyleableProperty&&) = delete;

		//! @brief Invokes the setter on the specified owner with the given value and `ValueSource::Local` value source
		//! @param owner The owner object
		//! @param value The value to set
		void InvokeSetter(Owner& owner, Value value)
		{
			(owner.*_setter)(value, ValueSource::Local);
		}

		//! @brief Invokes the setter on the specified owner with the given value and value source
		//! @param owner The owner object
		//! @param value The value to set
		//! @param source Determines value priority
		void InvokeSetter(Owner& owner, Value value, ValueSource source)
		{
			(owner.*_setter)(value, source);
		}

		//! @brief Invokes the getter on the specified owner and returns the value
		//! @param Owner the owner object
		//! @return The property value
		Value InvokeGetter(Owner& owner) const
		{
			return (owner.*_getter)();
		}

	private:
		Setter _setter;
		Getter _getter;
	};

	template<typename TOwner, typename TValue>
	StyleableProperty(void(TOwner::*)(TValue, ValueSource), TValue(TOwner::*)() const) -> StyleableProperty<TOwner, TValue>;
}