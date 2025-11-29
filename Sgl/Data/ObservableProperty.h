#pragma once

#include <concepts>
#include "SglProperty.h"

namespace Sgl
{
	template<typename TOwner, typename TValue>
	class ObservableProperty : public SglProperty
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using PropertySetter = void(Owner::*)(Value);
		using PropertyGetter = TValue(Owner::*)() const;
	private:
		PropertySetter _setter;
		PropertyGetter _getter;
	public:
		ObservableProperty(PropertySetter setter, PropertyGetter getter):
			_setter(setter), 
			_getter(getter)
		{}
		ObservableProperty(const ObservableProperty&) = default;
		ObservableProperty(ObservableProperty&&) = default;
		~ObservableProperty() = default;

		void Set(Owner& owner, Value value)
		{
			(owner.*_setter)(value);
		}

		Value Get(Owner& owner) const
		{
			return (owner.*_getter)();
		}
	};

	template<typename TOwner, typename TValue>
	ObservableProperty(void(TOwner::*)(TValue), TValue(TOwner::*)() const) -> ObservableProperty<TOwner, TValue>;
}