#pragma once

#include <typeinfo>
#include <utility>

namespace Sgl
{
	class VoidPtr final
	{
	private:
		void* _value;
	public:
		VoidPtr():
			_value(nullptr)
		{}

		VoidPtr(std::nullptr_t):
			VoidPtr()
		{}

		VoidPtr(const VoidPtr& other):
			_value(other._value)
		{}

		VoidPtr(VoidPtr&& other) noexcept:
			_value(std::exchange(other._value, nullptr))
		{}

		template<typename T>
		VoidPtr(T& value):
			_value(&value)
		{}

		bool IsEmpty() const
		{
			return _value == nullptr;
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

		friend bool operator==(VoidPtr left, VoidPtr right)
		{
			return left._value == right._value;
		}

		template<typename T>
		VoidPtr& operator=(T& value)
		{
			_value = &value;
			return *this;
		}

		VoidPtr& operator=(const VoidPtr& other)
		{
			_value = other._value;
			return *this;
		}

		VoidPtr& operator=(VoidPtr&& other) noexcept
		{
			_value = std::exchange(other._value, nullptr);
			return *this;
		}

		friend class ConstVoidPtr;
	};

	class ConstVoidPtr final
	{
	private:
		const void* _value;
	public:
		ConstVoidPtr():
			_value(nullptr)
		{}

		ConstVoidPtr(std::nullptr_t):
			ConstVoidPtr()
		{}

		template<typename T>
		ConstVoidPtr(const T& value):
			_value(&value)
		{}

		ConstVoidPtr(VoidPtr other):
			_value(other._value)
		{}

		ConstVoidPtr(const ConstVoidPtr& other):
			_value(other._value)
		{}

		ConstVoidPtr(ConstVoidPtr&& other) noexcept:
			_value(std::exchange(other._value, nullptr))
		{}

		bool IsEmpty() const
		{
			return _value == nullptr;
		}

		template<typename T>
		const T& As() const
		{
			return *static_cast<const T*>(_value);
		}

		friend bool operator==(ConstVoidPtr left, ConstVoidPtr right)
		{
			return left._value == right._value;
		}

		template<typename T>
		ConstVoidPtr& operator=(T& value)
		{
			_value = &value;
			return *this;
		}

		ConstVoidPtr& operator=(const ConstVoidPtr& other)
		{
			_value = other._value;
			return *this;
		}

		ConstVoidPtr& operator=(ConstVoidPtr&& other) noexcept
		{
			_value = std::exchange(other._value, nullptr);
			return *this;
		}
	};
}