#pragma once
#include <functional>
#include "Nullable.h"

namespace Sgl
{
	class object final
	{
	public:
		object():
			_value(nullptr), _type(typeid(nullptr))
		{}

		object(std::nullptr_t):
			object()
		{}

		template<typename T>
		object(T& value) :
			_value(&value), _type(typeid(T))
		{}

		template<typename T>
		bool Is() const
		{
			return _type == typeid(T);
		}

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

		template<typename T>
		Nullable<T> TryAs() noexcept
		{
			return Is<T>() ? &As<T>() : nullptr;
		}

		template<typename T>
		Nullable<const T> TryAs() const noexcept
		{
			return Is<T>() ? &As<T>() : nullptr;
		}

		friend bool operator==(object left, object right)
		{
			return left._value == right._value;
		}

		template<typename T>
		object& operator=(T& value)
		{
			_type = typeid(T);
			_value = &value;
			return *this;
		}

		object& operator=(object ref)
		{
			_type = ref._type;
			_value = ref._value;
			return *this;
		}
	private:
		std::reference_wrapper<const std::type_info> _type;
		void* _value;

		friend class const_object;
	};

	class const_object final
	{
	public:
		const_object():
			_value(nullptr), _type(typeid(nullptr))
		{}

		const_object(std::nullptr_t):
			const_object()
		{}

		const_object(const object object):
			_value(object._value), _type(object._type)
		{}

		template<typename T>
		const_object(const T& value) :
			_value(&value), _type(typeid(T))
		{}

		template<typename T>
		bool Is() const
		{
			return _type == typeid(T);
		}

		template<typename T>
		const T& As() const
		{
			return *static_cast<const T*>(_value);
		}

		template<typename T>
		Nullable<const T> TryAs() const noexcept
		{
			return Is<T>() ? &As<T>() : nullptr;
		}

		friend bool operator==(const_object left, const_object right)
		{
			return left._value == right._value;
		}

		template<typename T>
		const_object& operator=(T& value)
		{
			_type = typeid(T);
			_value = &value;
			return *this;
		}

		const_object& operator=(const_object ref)
		{
			_type = ref._type;
			_value = ref._value;
			return *this;
		}
	private:
		std::reference_wrapper<const std::type_info> _type;
		const void* _value;
	};
}