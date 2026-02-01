#pragma once

#include <stdint.h>
#include <concepts>
#include "Property.h"

namespace Sgl
{
	template<typename TOwner, typename TValue>
	class DirectProperty : public Property<TOwner, TValue>
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using Setter = void(Owner::*)(Value);
		using Getter = TValue(Owner::*)() const;
	
		DirectProperty(Setter setter, Getter getter):
			_setter(setter),
			_getter(getter)
		{}
		DirectProperty(const DirectProperty&) = delete;
		DirectProperty(DirectProperty&&) = delete;

		void InvokeSetter(Owner& owner, Value value) final
		{
			(owner.*_setter)(value);
		}

		Value InvokeGetter(Owner& owner) const final
		{
			return (owner.*_getter)();
		}
	private:
		Setter _setter;
		Getter _getter;
	};

	template<typename TOwner, typename TValue>
	DirectProperty(void(TOwner::*)(TValue), TValue(TOwner::*)() const) -> DirectProperty<TOwner, TValue>;
}