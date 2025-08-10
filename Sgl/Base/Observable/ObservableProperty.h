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

		const PropertyId Id = _id++;
		const PropertySetter Setter;
	private:
		static inline PropertyId _id = 0;
	public:
		ObservableProperty(PropertySetter setter): Setter(setter) {}
		ObservableProperty(const ObservableProperty&) = delete;
		ObservableProperty(ObservableProperty&&) = delete;
		~ObservableProperty() = default;
	};
}