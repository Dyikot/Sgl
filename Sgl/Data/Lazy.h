#pragma once

#include <memory>
#include <optional>
#include "../Tools/Delegates.h"

namespace Sgl
{
	template<typename T>
	struct DefaulValueFactory
	{
		std::optional<T> Value;

		constexpr DefaulValueFactory() = default;
		constexpr DefaulValueFactory(std::convertible_to<T> auto&& value):
			Value(std::forward<decltype(value)>(value))
		{}

		constexpr T operator()()
		{
			if(Value.has_value())
			{
				return Value.value();
			}

			return T{};
		}
	};

	template <typename T, Func<T> TFactory = DefaulValueFactory<T>>
	class Lazy final
	{
	private:
		mutable std::unique_ptr<T> _value;
		mutable TFactory _valueFactory;
	public:
		Lazy() requires std::default_initializable<T>:
			_valueFactory()
		{}

		Lazy(std::convertible_to<T> auto&& value):
			_valueFactory(std::forward<decltype(value)>(value))
		{}

		Lazy(TFactory factory):
			_valueFactory(std::move(factory))
		{}

		Lazy(const Lazy&) = delete;
		Lazy(Lazy&&) = delete;

		T& Value()
		{
			TryCreateValue();
			return *_value;
		}

		const T& Value() const
		{
			TryCreateValue();
			return *_value;
		}

		bool IsValueCreated() const 
		{ 
			return _value;
		}

		operator T& () { return Value(); }
		operator const T& () const { return Value(); }

		T* operator->() { return &Value(); }
		const T* operator->() const { return &Value(); }
	private:
		void TryCreateValue() const
		{
			if(!_value)
			{
				_value = std::make_unique<T>(_valueFactory());
			}
		}
	};

	template<std::invocable TFactory>
	Lazy(TFactory) -> Lazy<std::invoke_result_t<TFactory>, std::decay_t<TFactory>>;

	template<std::copy_constructible T> requires (!std::invocable<T>)
	Lazy(T&&) -> Lazy<T>;
}