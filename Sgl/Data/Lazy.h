#pragma once

#include <optional>
#include <functional>

namespace Sgl
{
	template <typename T>
	class Lazy
	{
	private:
		mutable std::optional<T> _value;
		std::function<T()> _valueFactory;
	public:
		Lazy():
			_valueFactory([] { return T{}; })
		{}

		template<typename TInvocable>
			requires std::is_invocable_r_v<T, TInvocable>
		Lazy(TInvocable&& valueFactory) :
			_valueFactory(std::forward<TInvocable>(valueFactory))
		{}

		template<typename TValue>
			requires std::constructible_from<T, TValue> && !std::is_reference_v<TValue>
		Lazy(TValue&& value) :
			_valueFactory([v = std::forward<TValue>(value)] { return v; })
		{}

		Lazy(Lazy&& other) noexcept:
			_value(std::exchange(other._value, std::nullopt)),
			_valueFactory(std::move(other._valueFactory))
		{}

		const T& Value() const
		{
			if(!_value)
			{
				_value = _valueFactory();
			}

			return _value.value();
		}

		bool IsValueCreated() const { return _value; }

		Lazy& operator=(Lazy&& other) noexcept
		{
			_value = std::exchange(other._value, std::nullopt);
			_valueFactory = std::move(other._valueFactory);
			return *this;
		}
	};
}