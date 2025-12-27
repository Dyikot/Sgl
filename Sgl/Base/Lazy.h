#pragma once

#include <variant>
#include "Delegate.h"

namespace Sgl
{
	template <typename T>
	class Lazy final
	{
	private:
		mutable std::variant<Func<T>, T> _value;
	public:
		Lazy() requires std::default_initializable<T>:
			_value([] { return T(); })
		{}

		explicit Lazy(Func<T> dataFactory):
			_value(std::move(dataFactory))
		{}

		Lazy(const Lazy&) = default;
		Lazy(Lazy&&) noexcept = default;

		T& GetValue() { return GetDataValue(); }
		const T& GetValue() const { return GetDataValue(); }

		bool IsValueCreated() const noexcept { return _value.index() == 1; }

		Lazy& operator=(const Lazy&) = default;
		Lazy& operator=(Lazy&&) noexcept = default;

		T& operator*() { return GetValue(); }
		const T& operator*() const { return GetValue(); }

		T* operator->() {  return &GetDataValue(); }
		const T* operator->() const { return &GetDataValue(); }
	private:
		T& GetDataValue() const
		{
			if(_value.index() == 0)
			{
				_value = std::get<Func<T>>(_value)();
			}

			return std::get<T>(_value);
		}
	};
}