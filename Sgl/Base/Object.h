#pragma once

#include <memory>

namespace Sgl
{
	class object final
	{
	private:
		std::shared_ptr<void> _value;
		std::reference_wrapper<const std::type_info> _type;
	public:
		object() noexcept:
			_value(nullptr),
			_type(typeid(nullptr))
		{}

		object(std::nullptr_t) noexcept:
			object()
		{}

		template<typename T>
		object(T&& value):
			_value(std::make_shared<std::decay_t<T>>(std::forward<T>(value))),
			_type(typeid(std::decay_t<T>))
		{}

		template<typename T>
		object(std::shared_ptr<T> value):
			_value(value),
			_type(typeid(T))
		{}

		object(const object& other):
			_value(other._value),
			_type(other._type)
		{}

		object(object&& other) noexcept:
			_value(std::move(other._value)),
			_type(typeid(nullptr))
		{}

		template<typename T>
		bool Is() const
		{
			return _type == typeid(T);
		}

		template<typename T>
		bool Is(T*& out)
		{
			if(_type == typeid(T))
			{
				out = &As<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		bool Is(const T*& out) const
		{
			if(_type == typeid(T))
			{
				out = &As<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		bool Is(std::shared_ptr<T>& out)
		{
			if(_type == typeid(T))
			{
				out = &AsShared<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		bool Is(const std::shared_ptr<T>& out) const
		{
			if(_type == typeid(T))
			{
				out = &AsShared<T>();
				return true;
			}

			return false;
		}

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
			_type = other._type;
			return *this;
		}

		object& operator=(object&& other) noexcept
		{
			_value = std::move(other._value);
			_type = typeid(nullptr);
			return *this;
		}
	};
}