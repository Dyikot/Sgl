#pragma once

#include <concepts>

namespace Sgl
{
	template<typename TOwner, typename T, typename TInput = T>
		requires std::constructible_from<T, TInput> && std::copyable<T>
	class BindableProperty
	{
	public:
		using PropertyId = size_t;
		using Type = T;
		using InputType = TInput;
		using Setter = void(TOwner::*)(TInput);
		using OwnerType = TOwner;

		const PropertyId Id = _id++;
		const Setter PropertySetter;
	private:
		static inline PropertyId _id = 0;
	public:
		BindableProperty(Setter setter):
			PropertySetter(setter)
		{}

		BindableProperty(const BindableProperty&) = default;
		BindableProperty(BindableProperty&&) = default;
		~BindableProperty() = default;
	};
}