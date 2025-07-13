#pragma once

#include <memory>

namespace Sgl
{
	class object final
	{
	private:
		std::shared_ptr<void> _value;
	public:
		object() noexcept:
			_value(nullptr)
		{}

		object(std::nullptr_t) noexcept:
			object()
		{}

		template<typename T>
		object(T&& value):
			_value(std::make_shared<std::decay_t<T>>(std::forward<T>(value)))
		{}

		template<typename T>
		object(std::shared_ptr<T> value):
			_value(value)
		{}

		object(const object& other):
			_value(other._value)
		{}

		object(object&& other) noexcept:
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

		object& operator=(const object& other)
		{
			_value = other._value;
			return *this;
		}

		object& operator=(object&& other) noexcept
		{
			_value = std::move(other._value);
			return *this;
		}
	};
}