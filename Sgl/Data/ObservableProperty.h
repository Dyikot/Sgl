#pragma once

#include <stdint.h>

namespace Sgl
{
	class ObservableObject;

	class ObservablePropertyBase
	{
	private:
		static inline uint32_t _nextId = 0;
	public:
		const uint32_t Id = _nextId++;
	public:
		ObservablePropertyBase() = default;
		ObservablePropertyBase(const ObservablePropertyBase&) = delete;
		ObservablePropertyBase(ObservablePropertyBase&&) = delete;

		friend class ObservableObject;
	};

	template<typename TOwner, typename TValue>
	class ObservableProperty : public ObservablePropertyBase
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
		ObservableProperty(const ObservableProperty&) = delete;
		ObservableProperty(ObservableProperty&&) = delete;

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