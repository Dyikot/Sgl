#pragma once
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include <string_view>
#include <stdexcept>
#include "Nullable.h"

namespace Sgl
{
	class Any final
	{
	private:
		template<typename T>
		struct ValueContainer;

		struct IValueContainer
		{
			virtual ~IValueContainer() = default;

			virtual const std::type_info& Type() const = 0;
			virtual IValueContainer* Copy() const = 0;

			template<typename T>
			T& Get() { return static_cast<ValueContainer<T>*>(this)->Value; }

			template<typename T>
			const T& Get() const { return static_cast<ValueContainer<T>*>(this)->Value; }
		};

		template<typename T>
		struct ValueContainer: public IValueContainer
		{
			T Value;

			template<typename... TArgs>
			ValueContainer(TArgs&&... args):
				Value(std::forward<TArgs>(args)...)
			{}

			const std::type_info& Type() const override { return typeid(Value); }
			IValueContainer* Copy() const override { return new ValueContainer<T>(*this); }
		};
	public:
		template<typename TValue, typename... TArgs>
		static Any New(TArgs&&... args)
		{
			Any any;
			any._value = new ValueContainer<TValue>(std::forward<TArgs>(args)...);
			return any;
		}

		Any() noexcept = default;

		template<typename T> requires (!std::same_as<std::decay_t<T>, Any>)
		Any(T&& value):
			_value(new ValueContainer<std::decay_t<T>>(std::forward<T>(value)))
		{}

		Any(const Any& any):
			_value(any._value->Copy())
		{}

		Any(Any&& any) noexcept:
			_value(std::exchange(any._value, nullptr))
		{}

		~Any() noexcept { delete _value; }

		const std::type_info& Type() const { return _value->Type(); }

		template<typename T>
		bool Is() const { return Type() == typeid(T); }
		bool Is(const std::type_info& type) const { return Type() == type; }
		bool Is(std::string_view type) const { return Type().name() == type; }

		template<typename T>
		T& As() { return _value->Get<T>(); }

		template<typename T>
		const T& As() const { return _value->Get<T>(); }

		template<typename T>
		Nullable<T> TryAs() noexcept { return HasValue() && Is<T>() ? &As<T>() : nullptr; }

		template<typename T>
		Nullable<const T> TryAs() const noexcept { return HasValue() && Is<T>() ? &As<T>() : nullptr; }
	
		bool HasValue() const noexcept { return _value; }

		template<typename T> requires (!std::same_as<std::decay_t<T>, Any>)
		Any& operator=(T&& value)
		{
			delete _value;
			_value = new ValueContainer<std::decay_t<T>>(std::forward<T>(value));
			return *this;
		}

		Any& operator=(const Any& any)
		{
			delete _value;
			_value = any._value->Copy();
			return *this;
		}

		Any& operator=(Any&& any) noexcept
		{
			delete _value;
			_value = std::exchange(any._value, nullptr);
			return *this;
		}

		operator bool() const noexcept { return HasValue(); }
	private:
		IValueContainer* _value = nullptr;
	};

	class AnyRef final
	{
	public:
		template<typename T> 
		AnyRef(T& value)
			: _value(&value), _type(typeid(T))
		{}

		template<typename T>
		bool Is() const { return _type == typeid(T); }
		
		template<typename T>
		T& As() { return *static_cast<T*>(_value); }

		template<typename T>
		const T& As() const { return *static_cast<const T*>(_value); }

		template<typename T>
		Nullable<T> TryAs() noexcept { return Is<T>() ? &As<T>() : nullptr; }

		template<typename T>
		Nullable<const T> TryAs() const noexcept { return Is<T>() ? &As<T>() : nullptr; }

		friend bool operator==(const AnyRef& left, const AnyRef& right)
		{
			return left._type == right._type;
		}

		friend bool operator!=(const AnyRef& left, const AnyRef& right)
		{
			return !operator==(left, right);
		}

		template<typename T>
		AnyRef& operator=(T& value)
		{
			if(!Is<T>())
			{
				throw std::invalid_argument("Wrong reference type");
			}

			_value = &value;
			return *this;
		}

		AnyRef& operator=(const AnyRef& ref)
		{
			if(*this != ref)
			{
				throw std::invalid_argument("Wrong reference type");
			}

			_value = ref._value;
			return *this;
		}
	private:
		const type_info& _type;
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
			Base::emplace(key, Any::New<TValue>(std::forward<TArgs>(args)...));
		}
	};
}