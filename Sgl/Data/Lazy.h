#pragma once

#include <memory>
#include <functional>

namespace Sgl
{
	template <typename T>
	class Lazy
	{
	private:
		mutable std::unique_ptr<T> _value;
		std::function<T()> _valueFactory;
	public:
		Lazy():
			_valueFactory([] { return T{}; })
		{}

		Lazy(const Lazy&) = delete;
		Lazy(Lazy&&) = delete;

		template<typename TInvocable>
			requires std::is_invocable_r_v<T, TInvocable>
		Lazy(TInvocable&& valueFactory) :
			_valueFactory(std::forward<TInvocable>(valueFactory))
		{}

		template<typename TValue>
			requires std::constructible_from<T, TValue> && (!std::is_reference_v<TValue>)
		Lazy(TValue&& value) :
			_valueFactory([v = std::forward<TValue>(value)] { return v; })
		{}

		T& Value()
		{
			if(!_value)
			{
				_value = std::make_unique<T>(_valueFactory());
			}

			return *_value;
		}

		const T& Value() const { return Value(); }
		bool IsValueCreated() const { return _value; }

		operator T& () { return Value(); }
		operator const T& () const { return Value(); }

		T* operator->() { return &Value(); }
		const T* operator->() const { return &Value(); }
	};
}