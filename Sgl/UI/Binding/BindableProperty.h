#pragma once

#include <concepts>

namespace Sgl
{
	template<typename TOwner, typename T, typename TInput = T>
		requires std::constructible_from<T, TInput> && std::copyable<T>
	class BindableProperty
	{
	public:
		using Type = T;
		using InputType = TInput;
		using Setter = void(TOwner::*)(TInput);
		using OwnerType = TOwner;

		const size_t Id;
		const T DefaultValue;
		const Setter PropertySetter;
	private:
		static inline size_t _id = 0;
	public:
		BindableProperty(Setter setter) requires std::default_initializable<T>:
			Id(_id++),
			DefaultValue(),
			PropertySetter(setter)
		{}

		BindableProperty(Setter setter, TInput value):
			Id(_id++),
			DefaultValue(value),
			PropertySetter(setter)
		{}

		BindableProperty(const BindableProperty&) = default;
		BindableProperty(BindableProperty&&) = default;
		~BindableProperty() = default;
	};
}