#pragma once

#include <stdint.h>

namespace Sgl
{
	class INotityPropertyChanged;

	class SglPropertyBase
	{
	public:
		SglPropertyBase() = default;
		SglPropertyBase(const SglPropertyBase&) = delete;
		SglPropertyBase(SglPropertyBase&&) = delete;

		friend bool operator==(const SglPropertyBase& left, const SglPropertyBase& right)
		{
			return &left == &right;
		}
	};

	template<typename TOwner, typename TValue>
	class SglProperty : public SglPropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using PropertySetter = void(Owner::*)(Value);
		using PropertyGetter = TValue(Owner::*)() const;
	public:
		PropertySetter Setter;
		PropertyGetter Getter;
	public:
		SglProperty(PropertySetter setter, PropertyGetter getter):
			Setter(setter), 
			Getter(getter)
		{}
		SglProperty(const SglProperty&) = delete;
		SglProperty(SglProperty&&) = delete;

		void InvokeSetter(Owner& owner, Value value)
		{
			(owner.*Setter)(value);
		}

		Value InvokeGetter(Owner& owner) const
		{
			return (owner.*Getter)();
		}
	};

	template<typename TOwner, typename TValue>
	SglProperty(void(TOwner::*)(TValue), TValue(TOwner::*)() const) -> SglProperty<TOwner, TValue>;
}