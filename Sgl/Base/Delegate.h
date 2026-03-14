#pragma once

#include <utility>
#include <typeinfo>

namespace Sgl
{
	template<typename T>
	class Delegate;

	/// <summary>
	/// A generic delegate class that can store and invoke callable objects with specified signature.
	/// </summary>
	template<typename TReturn, typename... TArgs>
	class Delegate<TReturn(TArgs...)>
	{
	private:
		struct IImpl
		{
		public:
			virtual ~IImpl() = default;

			virtual IImpl* Copy() const = 0;
			virtual const std::type_info& Type() const = 0;
			virtual bool Equals(const IImpl& other) const = 0;
			virtual TReturn operator()(TArgs&&... args) = 0;
		};

		template<typename TCallable>
		struct Impl : public IImpl
		{
		public:
			Impl(TCallable callable):
				Callable(std::move(callable))
			{}

			TCallable Callable;

			IImpl* Copy() const override
			{
				return new Impl<TCallable>(Callable);
			}

			const std::type_info& Type() const override
			{
				return typeid(TCallable);
			}

			bool Equals(const IImpl& other) const override
			{
				if(Type() != other.Type())
				{
					return false;
				}

				if constexpr(std::equality_comparable<TCallable>)
				{
					const auto& otherModel = static_cast<const Impl<TCallable>&>(other);
					return Callable == otherModel.Callable;
				}

				return false;
			}

			TReturn operator()(TArgs&&... args) override
			{
				return std::invoke(Callable, std::forward<TArgs>(args)...);
			}
		};
	public:
		/// <summary>
		/// Default constructor. Creates an empty delegate with no target.
		/// </summary>
		Delegate() noexcept = default;

		// <summary>
		/// Null pointer constructor. Creates an empty delegate.
		/// </summary>
		/// <param name="ptr"> - nullptr value.</param>
		Delegate(std::nullptr_t) noexcept {}

		/// <summary>
		/// Constructs a delegate with the specified callable object.
		/// </summary>
		/// <param name="func"> - The callable object to wrap.</param>
		template<typename TFunc> requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate(TFunc&& func):
			_impl(new Impl<std::decay_t<TFunc>>(std::forward<TFunc>(func)))
		{}

		/// <summary>
		/// Copy constructor. Creates a new delegate with a copy of the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to copy from.</param>
		Delegate(const Delegate& other):
			_impl(other.HasTarget() ? other._impl->Copy() : nullptr)
		{}

		/// <summary>
		/// Move constructor. Transfers ownership of the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to move from.</param>
		Delegate(Delegate&& other) noexcept:
			_impl(std::exchange(other._impl, nullptr))
		{}

		~Delegate()
		{
			delete _impl;
		}

		/// <summary>
		/// Resets the delegate to empty state, releasing any held callable.
		/// </summary>
		void Reset() noexcept
		{
			delete _impl;
			_impl = nullptr;
		}

		/// <summary>
		/// Checks whether the delegate has a target callable.
		/// </summary>
		/// <returns>True if the delegate has a target; otherwise, false.</returns>
		bool HasTarget() const noexcept
		{
			return _impl != nullptr;
		}

		/// <summary>
		/// Gets the type information of the target callable.
		/// </summary>
		/// <returns>A reference to the type_info object representing the target callable's type, or typeid(nullptr) if empty.</returns>
		const std::type_info& TargetType() const noexcept
		{
			return _impl ? _impl->Type() : typeid(nullptr);
		}

		/// <summary>
		/// Invokes the target callable with the specified arguments.
		/// </summary>
		/// <param name="args"> - The arguments to pass to the callable.</param>
		/// <returns>The result of invoking the callable.</returns>
		/// <exception cref="std::bad_function_call">Thrown if the delegate is empty.</exception>
		TReturn operator()(TArgs... args) const
		{
			return (*_impl)(std::forward<TArgs>(args)...);
		}

		/// <summary>
		/// Assignment operator for nullptr. Resets the delegate to empty state.
		/// </summary>
		/// <param name="ptr"> - nullptr value.</param>
		Delegate& operator=(nullptr_t) noexcept
		{
			delete _impl;
			_impl = nullptr;
			return *this;
		}

		// <summary>
		/// Assignment operator for callable objects.
		/// </summary>
		/// <param name="func"> - The callable object to assign.</param>
		template<typename TFunc> requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate& operator=(TFunc&& func)
		{
			delete _impl;
			_impl = new Impl<std::decay_t<TFunc>>(std::forward<TFunc>(func));
			return *this;
		}

		/// <summary>
		/// Copy assignment operator. Copies the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to copy from.</param>
		Delegate& operator=(const Delegate& other)
		{
			if(this != &other)
			{
				delete _impl;
				_impl = other.HasTarget() ? other._impl->Copy() : nullptr;
			}

			return *this;
		}

		/// <summary>
		/// Move assignment operator. Transfers ownership of the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to move from.</param>
		Delegate& operator=(Delegate&& other) noexcept
		{
			if(this != &other)
			{
				delete _impl;
				_impl = std::exchange(other._impl, nullptr);
			}

			return *this;
		}

		/// <summary>
		/// Checks whether the delegate has a target callable.
		/// </summary>
		/// <returns>True if the delegate has a target; otherwise, false.</returns>
		explicit operator bool() const noexcept
		{
			return _impl != nullptr;
		}

		/// <summary>
		/// Equality operator. Compares two delegates based on their target types.
		/// </summary>
		/// <param name="left"> - The first delegate to compare.</param>
		/// <param name="right"> - The second delegate to compare.</param>
		/// <returns>True if both delegates have targets of the same type; otherwise, false.</returns>
		friend bool operator==(const Delegate& left, const Delegate& right) noexcept
		{
			bool isLeftNull = left._impl == nullptr;
			bool isRightNull = right._impl == nullptr;

			if(isLeftNull || isRightNull)
			{
				return isLeftNull && isRightNull;
			}

			return left._impl->Equals(*right._impl);
		}
	private:
		IImpl* _impl = nullptr;
	};

	/// <summary>
	/// Represents a delegate that encapsulates a method that takes any number of arguments and does not return a value.
	/// </summary>
	template<typename... TArgs>
	using Action = Delegate<void(TArgs...)>;

	/// <summary>
	/// Represents a delegate that encapsulates a method that takes any number of arguments and returns a boolean value.
	/// </summary>
	template<typename... TArgs>
	using Predicate = Delegate<bool(TArgs...)>;

	/// <summary>
	/// Represents a delegate that encapsulates a method that takes any number of arguments and returns a value of the specified type.
	/// </summary>
	template<typename TResult, typename... TArgs>
	using Func = Delegate<TResult(TArgs...)>;
}
