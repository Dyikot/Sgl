#pragma once

#include <concepts>

namespace Sgl
{
	template<typename TOwner, typename TValue>
	class ObservableProperty
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using PropertyId = size_t;
		using PropertySetter = void(TOwner::*)(TValue);
		using PropertyGetter = TValue(TOwner::*)() const;

		const PropertyId Id = _id++;
		const PropertySetter Setter;
		const PropertyGetter Getter;
	private:
		static inline PropertyId _id = 0;
	public:
		ObservableProperty(PropertySetter setter, PropertyGetter getter) :
			Setter(setter), Getter(getter)
		{}
		ObservableProperty(const ObservableProperty&) = delete;
		ObservableProperty(ObservableProperty&&) = delete;
		~ObservableProperty() = default;
	};
}