#pragma once

#include <stdint.h>
#include "Property.h"

namespace Sgl
{
	/// <summary>
	/// Represents a direct property that wraps a getter and setter method pair.
	/// The getter and setter are invoked directly on the owner object without additional logic.
	/// </summary>
	template<typename TOwner, typename TValue>
	class DirectProperty : public PropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using Setter = void(Owner::*)(Value);
		using Getter = TValue(Owner::*)() const;

		/// <summary>
		/// Initializes a new instance with both getter and setter.
		/// </summary>
		/// <param name="setter"> - the setter method.</param>
		/// <param name="getter"> - the getter method.</param>
		DirectProperty(Setter setter, Getter getter):
			_setter(setter),
			_getter(getter)
		{}

		/// <summary>
		/// Initializes a new instance with getter only (read-only property).
		/// </summary>
		/// <param name="getter"> - the getter method.</param>
		DirectProperty(Getter getter):
			_setter(nullptr),
			_getter(getter)
		{}

		DirectProperty(const DirectProperty&) = delete;
		DirectProperty(DirectProperty&&) = delete;

		/// <summary>
		/// Invokes the setter on the specified owner with the given value.
		/// </summary>
		/// <param name="owner"> - the owner object.</param>
		/// <param name="value"> - the value to set.</param>
		void InvokeSetter(Owner& owner, Value value)
		{
			(owner.*_setter)(value);
		}

		/// <summary>
		/// Invokes the getter on the specified owner and returns the value.
		/// </summary>
		/// <param name="owner"> - the owner object.</param>
		/// <returns>The property value.</returns>
		Value InvokeGetter(Owner& owner) const
		{
			return (owner.*_getter)();
		}

		/// <summary>
		/// Determines whether this property has a setter (is writable).
		/// </summary>
		/// <returns>True if the property has a setter, false if read-only.</returns>
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