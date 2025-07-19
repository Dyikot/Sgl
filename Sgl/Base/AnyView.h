#pragma once

#include <typeinfo>
#include <utility>

namespace Sgl
{
	class AnyView final
	{
	private:
		void* _value;
	public:
		AnyView():
			_value(nullptr)
		{}

		AnyView(std::nullptr_t):
			AnyView()
		{}

		AnyView(const AnyView& other):
			_value(other._value)
		{}

		AnyView(AnyView&& other) noexcept:
			_value(std::exchange(other._value, nullptr))
		{}

		template<typename T>
		AnyView(T& value):
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

		friend bool operator==(AnyView left, AnyView right)
		{
			return left._value == right._value;
		}

		template<typename T>
		AnyView& operator=(T& value)
		{
			_value = &value;
			return *this;
		}

		AnyView& operator=(const AnyView& other)
		{
			_value = other._value;
			return *this;
		}

		AnyView& operator=(AnyView&& other) noexcept
		{
			_value = std::exchange(other._value, nullptr);
			return *this;
		}

		friend class AnyConstView;
	};

	class AnyConstView final
	{
	private:
		const void* _value;
	public:
		AnyConstView():
			_value(nullptr)
		{}

		AnyConstView(std::nullptr_t):
			AnyConstView()
		{}

		template<typename T>
		AnyConstView(const T& value):
			_value(&value)
		{}

		AnyConstView(AnyView other):
			_value(other._value)
		{}

		AnyConstView(const AnyConstView& other):
			_value(other._value)
		{}

		AnyConstView(AnyConstView&& other) noexcept:
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

		friend bool operator==(AnyConstView left, AnyConstView right)
		{
			return left._value == right._value;
		}

		template<typename T>
		AnyConstView& operator=(T& value)
		{
			_value = &value;
			return *this;
		}

		AnyConstView& operator=(const AnyConstView& other)
		{
			_value = other._value;
			return *this;
		}

		AnyConstView& operator=(AnyConstView&& other) noexcept
		{
			_value = std::exchange(other._value, nullptr);
			return *this;
		}
	};
}