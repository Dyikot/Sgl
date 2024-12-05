#pragma once
#include <typeinfo>

namespace Sgl
{
	template<typename T>
	class ValueContainer;

	class IValueContainer
	{
	public:
		virtual ~IValueContainer() = default;

		virtual const std::type_info& Type() const = 0;
		virtual IValueContainer* Copy() const = 0;

		template<typename T>
		T& Get() { return static_cast<ValueContainer<T>*>(this)->Value; }
	};

	template<typename T>
	class ValueContainer: public IValueContainer
	{
	public:
		T Value;
	public:
		template<typename... TArgs>
		ValueContainer(TArgs&&... args):
			Value(std::forward<TArgs>(args)...)
		{}
		ValueContainer(const T& value):
			Value(value)
		{}

		IValueContainer* Copy() const override { return new ValueContainer<T>(*this); }
		const std::type_info& Type() const override { return typeid(Value); }
	};

	class Any
	{
	public:
		template<typename TValue, typename... TArgs>
		static Any New(TArgs&&... args)
		{
			Any value;
			value._value = new ValueContainer<TValue>(std::forward<TArgs>(args)...);
			return value;
		}

		Any() noexcept = default;
		template<typename T>
		Any(const T& value):
			_value(new ValueContainer<T>(value)) {}
		Any(const Any& any):
			_value(any._value->Copy())
		{}
		Any(Any&& any) noexcept
		{
			_value = any._value;
			any._value = nullptr;
		}
		~Any() noexcept { delete _value; }

		const std::type_info& Type() const { return _value->Type(); }
		template<typename T>
		bool Is() const { return Type() == typeid(T); }
		template<typename T>
		T& As() const { return _value->Get<T>(); }
		template<typename T>
		T* TryAs() const { return Is<T>() ? &_value->Get<T>() : nullptr; }

		Any& operator=(const Any& any)
		{
			delete _value;
			_value = any._value->Copy();
			return *this;
		}
		Any& operator=(Any&& any) noexcept
		{
			delete _value;
			_value = any._value;
			any._value = nullptr;
			return *this;
		}

		operator bool() const noexcept { return _value != nullptr; }
	private:
		IValueContainer* _value = nullptr;
	};
}