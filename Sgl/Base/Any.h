#pragma once

#include <memory>

namespace Sgl
{
	class Any final
	{
	private:
		std::shared_ptr<void> _value;
	public:
		Any() noexcept:
			_value(nullptr)
		{}

		Any(std::nullptr_t) noexcept:
			Any()
		{}

		template<typename T>
		Any(T&& value):
			_value(std::make_shared<std::decay_t<T>>(std::forward<T>(value)))
		{}

		template<typename T>
		Any(std::shared_ptr<T> value):
			_value(value)
		{}

		Any(const Any& other):
			_value(other._value)
		{}

		Any(Any&& other) noexcept:
			_value(std::move(other._value))
		{}

		template<typename T>
		T& As()
		{
			return *static_cast<T*>(_value.get());
		}

		template<typename T>
		const T& As() const
		{
			return *static_cast<const T*>(_value.get());
		}

		template<typename T>
		std::shared_ptr<T> AsShared() const
		{
			return std::static_pointer_cast<T, void>(_value);
		}

		Any& operator=(const Any& other)
		{
			_value = other._value;
			return *this;
		}

		Any& operator=(Any&& other) noexcept
		{
			_value = std::move(other._value);
			return *this;
		}
	};
}