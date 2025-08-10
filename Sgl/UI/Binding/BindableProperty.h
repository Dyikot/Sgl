#pragma once

#include <concepts>

namespace Sgl
{
	template<typename TOwner, typename TValue>
	class BindableProperty
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
		BindableProperty(Setter setter): PropertySetter(setter) {}
		BindableProperty(const BindableProperty&) = default;
		BindableProperty(BindableProperty&&) = default;
		~BindableProperty() = default;
	};
}