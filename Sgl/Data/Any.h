#pragma once
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>
#include "Nullable.h"

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

			virtual std::unique_ptr<ValueContainerBase> Copy() const = 0;

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

			std::unique_ptr<ValueContainerBase> Copy() const override
			{
				return std::make_unique<ValueContainer<T>>(*this);
			}
		};
	public:
		Any() noexcept = default;

		template<typename T> requires (!std::same_as<std::decay_t<T>, Any>)
		Any(T&& value):
			_value(std::make_unique<ValueContainer<std::decay_t<T>>>(std::forward<T>(value)))
		{}

		Any(const Any& any):
			_value(any._value->Copy())
		{}

		Any(Any&& any) noexcept:
			_value(std::exchange(any._value, nullptr))
		{}

		template<typename TValue, typename... TArgs>
		friend Any CreateAny(TArgs&&... args);

		template<typename T>
		bool Is() const
		{
			return _value->Type == typeid(T);
		}

		bool Is(const std::type_info& type) const
		{ 
			return _value->Type == type;
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

		template<typename T>
		Nullable<T> TryAs() noexcept
		{
			return HasValue() && Is<T>() ? &As<T>() : nullptr;
		}

		template<typename T>
		Nullable<const T> TryAs() const noexcept 
		{
			return HasValue() && Is<T>() ? &As<T>() : nullptr;
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

		Any& operator=(const Any& any)
		{
			_value = any._value->Copy();
			return *this;
		}

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

	class AnyView final
	{
	public:
		template<typename T> 
		AnyView(T& value):
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

		friend bool operator==(const AnyView& left, const AnyView& right)
		{
			return left._value == right._value;
		}

		template<typename T>
		AnyView& operator=(T& value)
		{
			_type = typeid(T);
			_value = &value;
			return *this;
		}

		AnyView& operator=(const AnyView& ref)
		{
			_type = ref._type;
			_value = ref._value;
			return *this;
		}
	private:
		std::reference_wrapper<const std::type_info> _type;
		void* _value;
	};

	template<typename TKey>
	class AnyMap: public std::unordered_map<TKey, Any>
	{
	public:
		using Base = std::unordered_map<TKey, Any>;
	public:
		template<typename TValue, typename... TArgs>
		void Add(const TKey& key, TArgs&&... args)
		{
			Base::emplace(key, CreateAny<TValue>(std::forward<TArgs>(args)...));
		}
	};

	template<typename TValue, typename ...TArgs>
	Any CreateAny(TArgs && ...args)
	{
		Any any;
		any._value = std::make_unique<Any::ValueContainer<TValue>>(std::forward<TArgs>(args)...);
		return any;
	}
}