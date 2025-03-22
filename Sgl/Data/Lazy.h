#pragma once

#include <memory>
#include <functional>

namespace Sgl
{
	template <typename T>
	class Lazy final
	{
	private:
		struct IValueFactory
		{
			virtual ~IValueFactory() = default;
			virtual T operator()() const = 0;
		};

		template<typename TInvocable>
		struct ValueFactory: public IValueFactory
		{
			TInvocable Invocable;
			ValueFactory(TInvocable&& invocable): Invocable(std::forward<TInvocable>(invocable)) {}
			T operator()() const override { return Invocable(); }
		};

		mutable std::unique_ptr<T> _value;
		mutable std::unique_ptr<IValueFactory> _valueFactory;
	public:
		Lazy():
			Lazy([] { return T{}; })
		{}

		Lazy(const Lazy&) = delete;
		Lazy(Lazy&&) = delete;

		template<typename TFactory>
			requires std::is_invocable_r_v<T, TFactory>
		Lazy(TFactory&& valueFactory)
		{
			_valueFactory = std::make_unique<ValueFactory<TFactory>>(
				std::forward<TFactory>(valueFactory));
		}

		template<typename TValue>
			requires std::constructible_from<T, TValue> && (!std::is_reference_v<TValue>)
		Lazy(TValue&& value)
		{
			auto f = [v = std::forward<TValue>(value)] { return v; };
			using F = decltype(f);
			_valueFactory = std::make_unique<ValueFactory<F>>(std::forward<F>(f));
		}

		T& Value()
		{
			TryCreateValue();
			return *_value;
		}

		const T& Value() const
		{
			TryCreateValue();
			return *_value;
		}

		bool IsValueCreated() const { return _value; }

		operator T& () { return Value(); }
		operator const T& () const { return Value(); }

		T* operator->() { return &Value(); }
		const T* operator->() const { return &Value(); }
	private:
		void TryCreateValue() const
		{
			if(!_value)
			{
				_value = std::make_unique<T>((*_valueFactory)());
			}
		}
	};
}