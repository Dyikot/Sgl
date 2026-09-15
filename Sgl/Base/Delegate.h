#pragma once

#include <utility>
#include <typeinfo>

namespace Sgl
{
	template<typename T>
	class Delegate;

	//! @brief A generic delegate class that can store and invoke callable objects with specified signature
	template<typename TReturn, typename... TArgs>
	class Delegate<TReturn(TArgs...)>
	{
	private:
		struct ICallable
		{
		public:
			virtual ~ICallable() = default;
			virtual ICallable* Copy() const = 0;
			virtual const std::type_info& Type() const = 0;
			virtual bool Equals(const ICallable& other) const = 0;
			virtual TReturn operator()(TArgs&&... args) = 0;
		};

		template<typename T>
		struct Callable final: public ICallable
		{
		public:
			Callable(T object):
				Object(std::move(object))
			{}

			T Object;

			ICallable* Copy() const override
			{
				return new Callable<T>(Object);
			}

			const std::type_info& Type() const override
			{
				return typeid(T);
			}

			bool Equals(const ICallable& other) const override
			{
				if(Type() != other.Type())
				{
					return false;
				}
				if constexpr(std::equality_comparable<T>)
				{
					auto& otherCallable = static_cast<const Callable<T>&>(other);
					return Object == otherCallable.Object;
				}
				return false;
			}

			TReturn operator()(TArgs&&... args) override
			{
				return std::invoke(Object, std::forward<TArgs>(args)...);
			}
		};

	public:
		//! @brief Default constructor. Creates an empty delegate with no target
		Delegate() noexcept = default;

		//! @brief Creates an empty delegate with no target
		Delegate(std::nullptr_t) noexcept {}

		//! @brief Constructs a delegate with the specified callable object
		//! @param func The callable object to wrap
		template<typename TFunc> requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate(TFunc&& func):
			_callable(new Callable<std::decay_t<TFunc>>(std::forward<TFunc>(func)))
		{}

		//! @brief Copy constructor
		Delegate(const Delegate& other):
			_callable(other.HasTarget() ? other._callable->Copy() : nullptr)
		{}

		//! @brief Move constructor
		Delegate(Delegate&& other) noexcept:
			_callable(std::exchange(other._callable, nullptr))
		{}

		~Delegate()
		{
			delete _callable;
		}

		//! @brief Resets the delegate to empty state, releasing any held callable
		void Reset() noexcept
		{
			delete _callable;
			_callable = nullptr;
		}

		//! @brief Checks whether the delegate has a target callable
		//! @return True if the delegate has a target; otherwise, false
		bool HasTarget() const noexcept
		{
			return _callable != nullptr;
		}

		//! @brief Gets the type information of the target callable
		//! @return A reference to the type_info object representing the target callable's type, or typeid(nullptr) if empty
		const std::type_info& TargetType() const noexcept
		{
			return _callable ? _callable->Type() : typeid(nullptr);
		}

		//! @brief Invokes the target callable with the specified arguments
		//! @param args The arguments to pass to the callable
		//! @return The result of invoking the callable
		TReturn operator()(TArgs... args) const
		{
			return (*_callable)(std::forward<TArgs>(args)...);
		}

		//! @brief Assignment operator for nullptr. Resets the delegate to empty state
		//! @param ptr Nullptr value
		Delegate& operator=(nullptr_t) noexcept
		{
			delete _callable;
			_callable = nullptr;
			return *this;
		}

		//! @brief Assignment operator for callable objects
		//! @param func The callable object to assign
		template<typename TFunc> requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate& operator=(TFunc&& func)
		{
			delete _callable;
			_callable = new Callable<std::decay_t<TFunc>>(std::forward<TFunc>(func));
			return *this;
		}

		//! @brief Copy assignment operator. Copies the callable from another delegate
		//! @param other The delegate to copy from
		Delegate& operator=(const Delegate& other)
		{
			if(this != &other)
			{
				delete _callable;
				_callable = other.HasTarget() ? other._callable->Copy() : nullptr;
			}

			return *this;
		}

		//! @brief Move assignment operator. Transfers ownership of the callable from another delegate
		//! @param other The delegate to move from
		Delegate& operator=(Delegate&& other) noexcept
		{
			if(this != &other)
			{
				delete _callable;
				_callable = std::exchange(other._callable, nullptr);
			}

			return *this;
		}

		//! @brief Checks whether the delegate has a target callable
		//! @return True if the delegate has a target; otherwise, false
		explicit operator bool() const noexcept
		{
			return _callable != nullptr;
		}

		//! @brief Equality operator. Compares two delegates based on their target types
		//! @param left The first delegate to compare
		//! @param right The second delegate to compare
		//! @return True if both delegates have targets of the same type; otherwise, false
		friend bool operator==(const Delegate& left, const Delegate& right) noexcept
		{
			bool isLeftNull = left._callable == nullptr;
			bool isRightNull = right._callable == nullptr;
			
			if(isLeftNull || isRightNull)
			{
				return isLeftNull && isRightNull;
			}

			return left._callable->Equals(*right._callable);
		}

	private:
		ICallable* _callable = nullptr;
	};

	//! @brief Represents a delegate that encapsulates a method that takes any number of arguments and does not return a value
	template<typename... TArgs>
	using Action = Delegate<void(TArgs...)>;

	//! @brief Represents a delegate that encapsulates a method that takes any number of arguments and returns a boolean value
	template<typename... TArgs>
	using Predicate = Delegate<bool(TArgs...)>;

	//! @brief Represents a delegate that encapsulates a method that takes any number of arguments and returns a value of the specified type
	template<typename TResult, typename... TArgs>
	using Func = Delegate<TResult(TArgs...)>;
}