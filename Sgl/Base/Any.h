#pragma once

#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>
#include <memory>

namespace Sgl
{
	class Any final
	{		
	private:
		template<typename T>
		struct ValueContainer;

		struct ValueContainerBase
		{
		public:
			const std::type_info& Type;
		public:
			ValueContainerBase(const std::type_info& type):
				Type(type)
			{}

			virtual ~ValueContainerBase() = default;

			template<typename T>
			T& Get()
			{
				return static_cast<ValueContainer<T>*>(this)->Value;
			}

			template<typename T>
			const T& Get() const
			{
				return static_cast<ValueContainer<T>*>(this)->Value;
			}
		};

		template<typename T>
		struct ValueContainer: public ValueContainerBase
		{
		public:
			T Value;
		public:
			template<typename... TArgs>
			ValueContainer(TArgs&&... args):
				ValueContainerBase(typeid(T)),
				Value(std::forward<TArgs>(args)...)
			{}
		};
	public:
		template<typename TValue, typename ...TArgs>
		static Any New(TArgs&& ...args)
		{
			Any any;
			any._value = std::make_unique<Any::ValueContainer<TValue>>(std::forward<TArgs>(args)...);
			return any;
		}

		Any() noexcept = default;

		template<typename T> requires (!std::same_as<std::decay_t<T>, Any>)
		Any(T&& value):
			_value(std::make_unique<ValueContainer<std::decay_t<T>>>(std::forward<T>(value)))
		{}

		Any(const Any&) = delete;

		Any(Any&& any) noexcept:
			_value(std::exchange(any._value, nullptr))
		{}

		template<typename T>
		bool Is() const
		{
			return _value->Type == typeid(T);
		}

		template<typename T>
		bool Is(T*& out)
		{
			if(HasValue() && Is<T>())
			{
				out = &As<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		bool Is(const T*& out) const
		{
			if(HasValue() && Is<T>())
			{
				out = &As<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		T& As() 
		{ 
			return _value->Get<T>();
		}

		template<typename T>
		const T& As() const 
		{ 
			return _value->Get<T>();
		}
	
		bool HasValue() const noexcept
		{ 
			return _value.operator bool();
		}

		template<typename T> requires (!std::same_as<std::decay_t<T>, Any>)
		Any& operator=(T&& value)
		{
			_value = std::make_unique<ValueContainer<std::decay_t<T>>>(std::forward<T>(value));
			return *this;
		}

		Any& operator=(const Any&) = delete;

		Any& operator=(Any&& any) noexcept
		{
			_value = std::exchange(any._value, nullptr);
			return *this;
		}

		operator bool() const noexcept 
		{ 
			return HasValue();
		}
	private:
		std::unique_ptr<ValueContainerBase> _value;
	};
}