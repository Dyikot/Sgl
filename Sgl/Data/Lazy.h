#pragma once

#include <memory>
#include <optional>
#include "Delegates.h"

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
			_valueFactory([] { return T{}; })
		{}

		Lazy(T&& value):
			_valueFactory([v = std::move(value)] { return v; })
		{}

		Lazy(CFunc<T> auto&& factory):
			_valueFactory(std::forward<decltype(factory)>(factory))
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
				_valueFactory = nullptr;
			}
		}
	};
}