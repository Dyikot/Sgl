#pragma once

#include <concepts>

namespace Sgl
{
	inline size_t GenerateUniquePropertyId()
	{
		static size_t counter = 0;
		return counter++;
	}

	template<typename TOwner, typename TValue>
	class ObservableProperty
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using PropertyId = size_t;
		using PropertySetter = void(TOwner::*)(TValue);
		using PropertyGetter = TValue(TOwner::*)() const;

		const PropertyId Id;
		const PropertySetter Setter;
		const PropertyGetter Getter;
	public:
		ObservableProperty(PropertySetter setter, PropertyGetter getter):
			Setter(setter), 
			Getter(getter), 
			Id(GenerateUniquePropertyId())
		{}
		ObservableProperty(const ObservableProperty&) = default;
		ObservableProperty(ObservableProperty&&) = default;
		~ObservableProperty() = default;
	};

	template<typename TOwner, typename TValue>
	ObservableProperty(void(TOwner::*)(TValue), TValue(TOwner::*)() const) -> ObservableProperty<TOwner, TValue>;
}