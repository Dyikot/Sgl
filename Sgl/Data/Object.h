#pragma once

#include <typeinfo>

namespace Sgl
{
	class object final
	{
	private:
		void* _value;
	public:
		object():
			_value(nullptr)
		{}

		object(std::nullptr_t):
			object()
		{}

		object(const object& other):
			_value(other._value)
		{}

		object(object&& other) noexcept:
			_value(std::exchange(other._value, nullptr))
		{}

		template<typename T>
		object(T& value):
			_value(&value)
		{}

		template<typename T>
		T& As()
		{
			return *static_cast<T*>(_value);
		}

		template<typename T>
		const T& As() const
		{
			return *static_cast<const T*>(_value);
		}

		friend bool operator==(object left, object right)
		{
			return left._value == right._value;
		}

		template<typename T>
		object& operator=(T& value)
		{
			_value = &value;
			return *this;
		}

		object& operator=(const object& other)
		{
			_value = other._value;
			return *this;
		}

		object& operator=(object&& other) noexcept
		{
			_value = std::exchange(other._value, nullptr);
			return *this;
		}

		friend class const_object;
	};

	class const_object final
	{
	private:
		const void* _value;
	public:
		const_object():
			_value(nullptr)
		{}

		const_object(std::nullptr_t):
			const_object()
		{}

		template<typename T>
		const_object(const T& value):
			_value(&value)
		{}

		const_object(object other):
			_value(other._value)
		{}

		const_object(const const_object& other):
			_value(other._value)
		{}

		const_object(const_object&& other) noexcept:
			_value(std::exchange(other._value, nullptr))
		{}

		template<typename T>
		const T& As() const
		{
			return *static_cast<const T*>(_value);
		}

		friend bool operator==(const_object left, const_object right)
		{
			return left._value == right._value;
		}

		template<typename T>
		const_object& operator=(T& value)
		{
			_value = &value;
			return *this;
		}

		const_object& operator=(const const_object& other)
		{
			_value = other._value;
			return *this;
		}

		const_object& operator=(const_object&& other) noexcept
		{
			_value = std::exchange(other._value, nullptr);
			return *this;
		}
	};
}