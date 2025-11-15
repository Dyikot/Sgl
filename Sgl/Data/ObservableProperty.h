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
		using PropertySetter = void(TOwner::*)(TValue);
		using PropertyGetter = TValue(TOwner::*)() const;

		const PropertySetter Setter;
		const PropertyGetter Getter;
	public:
		ObservableProperty(PropertySetter setter, PropertyGetter getter):
			Setter(setter), 
			Getter(getter)
		{}
		ObservableProperty(const ObservableProperty&) = default;
		ObservableProperty(ObservableProperty&&) = default;
		~ObservableProperty() = default;
	};

	template<typename TOwner, typename TValue>
	ObservableProperty(void(TOwner::*)(TValue), TValue(TOwner::*)() const) -> ObservableProperty<TOwner, TValue>;
}