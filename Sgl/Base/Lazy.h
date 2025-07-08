#pragma once

#include <memory>
#include <tuple>
#include "Delegate.h"

namespace Sgl
{
	template <typename T>
	class Lazy final
	{
	private:
		mutable std::unique_ptr<T> _value;
		mutable Func<T> _valueFactory;
	public:
		Lazy() requires std::default_initializable<T>:
			_valueFactory([] { return T(); })
		{}

		template<typename... TArgs> requires std::constructible_from<T, TArgs...>
		Lazy(TArgs&&... args)
		{
			_valueFactory = [args = std::make_tuple(std::forward<TArgs>(args)...)]
			{
				return std::apply(
					[](auto&&... args)
					{
						return T(std::forward<decltype(args)>(args)...);
					}, 
					std::move(args)
				);
			};
		}

		Lazy(Func<T> valueFactory):
			_valueFactory(std::move(valueFactory))
		{}

		Lazy(const Lazy& other) = delete;

		Lazy(Lazy&& other) noexcept:
			_value(std::move(other._value)), _valueFactory(std::move(other._valueFactory))
		{}

		T& Value()
		{
			CreateValueIfNull();
			return *_value;
		}

		const T& Value() const
		{
			CreateValueIfNull();
			return *_value;
		}

		bool IsValueCreated() const 
		{ 
			return _value.operator bool();
		}

		T& operator*() { return Value(); }
		const T& operator*() const { return Value(); }

		T* operator->() {  return &Value(); }
		const T* operator->() const { return &Value(); }
	private:
		void CreateValueIfNull() const
		{
			if(!_value)
			{
				_value = std::make_unique<T>(_valueFactory());
				_valueFactory = nullptr;
			}
		}
	};
}