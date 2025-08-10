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
		using Setter = void(TOwner::*)(TValue);

		const PropertyId Id = _id++;
		const Setter PropertySetter;
	private:
		static inline PropertyId _id = 0;
	public:
		ObservableProperty(Setter setter): PropertySetter(setter) {}
		ObservableProperty(const ObservableProperty&) = delete;
		ObservableProperty(ObservableProperty&&) = delete;
		~ObservableProperty() = default;
	};
}