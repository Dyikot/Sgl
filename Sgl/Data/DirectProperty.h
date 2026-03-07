#pragma once

#include <stdint.h>
#include "Property.h"

namespace Sgl
{
	template<typename TOwner, typename TValue>
	class DirectProperty : public PropertyBase
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
		DirectProperty(Getter getter):
			_setter(nullptr),
			_getter(getter)
		{}
		DirectProperty(const DirectProperty&) = delete;
		DirectProperty(DirectProperty&&) = delete;

		void InvokeSetter(Owner& owner, Value value)
		{
			(owner.*_setter)(value);
		}

		Value InvokeGetter(Owner& owner) const
		{
			return (owner.*_getter)();
		}

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