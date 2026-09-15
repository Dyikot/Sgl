#pragma once

#include <stdint.h>
#include "Property.h"

namespace Sgl
{
	//! @brief Represents a direct property that wraps a getter and setter method pair
	template<typename TOwner, typename TValue>
	class DirectProperty : public PropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using Setter = void(Owner::*)(Value);
		using Getter = TValue(Owner::*)() const;
	public:
		//! @brief Initializes a new instance with both getter and setter
		//! @param setter The setter method
		//! @param getter The getter method
		DirectProperty(Setter setter, Getter getter):
			_setter(setter),
			_getter(getter)
		{}

		//! @brief Initializes a new instance with getter only (read-only property)
		//! @param getter The getter method
		DirectProperty(Getter getter):
			_setter(nullptr),
			_getter(getter)
		{}

		DirectProperty(const DirectProperty&) = delete;
		DirectProperty(DirectProperty&&) = delete;

		//! @brief Invokes the setter on the specified owner with the given value
		//! @param owner The owner object
		//! @param value The value to set
		void InvokeSetter(Owner& owner, Value value)
		{
			(owner.*_setter)(value);
		}

		//! @brief Invokes the getter on the specified owner and returns the value
		//! @param Owner the owner object
		//! @return The property value
		Value InvokeGetter(Owner& owner) const
		{
			return (owner.*_getter)();
		}

		//! @brief Determines whether this property has a setter (is writable)
		//! @return True if the property has a setter, false if read-only
		bool HasSetter() const noexcept
		{
			return _setter != nullptr;
		}

	private:
		Setter _setter;
		Getter _getter;
	};

	template<typename TOwner, typename TValue>
	DirectProperty(void(TOwner::*)(TValue), TValue(TOwner::*)() const) -> DirectProperty<TOwner, TValue>;

	template<typename TOwner, typename TValue>
	DirectProperty(TValue(TOwner::*)() const) -> DirectProperty<TOwner, TValue>;
}