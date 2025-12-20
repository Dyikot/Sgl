#pragma once

#include <type_traits>
#include <memory>

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
		struct Concept
		{
		public:
			virtual ~Concept() = default;

			virtual std::unique_ptr<Concept> Copy() const = 0;
			virtual const std::type_info& Type() const = 0;
			virtual bool Equals(const Concept& other) const = 0;
			virtual TReturn operator()(TArgs... args) = 0;
		};

		template<typename TCallable>
		struct Model : public Concept
		{
		public:
			TCallable Callable;
		public:
			Model(TCallable callable):
				Callable(std::move(callable))
			{}

			std::unique_ptr<Concept> Copy() const override
			{
				return std::make_unique<Model>(Callable);
			}

			const std::type_info& Type() const override
			{
				return typeid(TCallable);
			}

			bool Equals(const Concept& other) const override
			{
				if(Type() != other.Type())
				{
					return false;
				}

				if constexpr(std::equality_comparable<TCallable>)
				{
					const auto& otherModel = static_cast<const Model<TCallable>&>(other);
					return Callable == otherModel.Callable;
				}

				return false;
			}

			TReturn operator()(TArgs... args) override
			{
				return std::invoke(Callable, std::forward<TArgs>(args)...);
			}
		};

		std::unique_ptr<Concept> _concept;
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
			_concept(std::make_unique<Model<std::decay_t<TFunc>>>(std::forward<TFunc>(func)))
		{}

		/// <summary>
		/// Copy constructor. Creates a new delegate with a copy of the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to copy from.</param>
		Delegate(const Delegate& other):
			_concept(other.HasTarget() ? other._concept->Copy() : nullptr)
		{}

		/// <summary>
		/// Move constructor. Transfers ownership of the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to move from.</param>
		Delegate(Delegate&& other) noexcept:
			_concept(std::exchange(other._concept, nullptr))
		{}

		/// <summary>
		/// Resets the delegate to empty state, releasing any held callable.
		/// </summary>
		void Reset() noexcept
		{		
			_concept.reset();
		}

		/// <summary>
		/// Checks whether the delegate has a target callable.
		/// </summary>
		/// <returns>True if the delegate has a target; otherwise, false.</returns>
		bool HasTarget() const noexcept
		{		
			return _concept != nullptr;
		}

		/// <summary>
		/// Gets the type information of the target callable.
		/// </summary>
		/// <returns>A reference to the type_info object representing the target callable's type, or typeid(nullptr) if empty.</returns>
		const std::type_info& TargetType() const noexcept
		{
			return _concept ? _concept->Type() : typeid(nullptr);
		}

		/// <summary>
		/// Invokes the target callable with the specified arguments.
		/// </summary>
		/// <param name="args"> - The arguments to pass to the callable.</param>
		/// <returns>The result of invoking the callable.</returns>
		/// <exception cref="std::bad_function_call">Thrown if the delegate is empty.</exception>
		TReturn operator()(TArgs... args) const
		{
			return (*_concept)(std::forward<TArgs>(args)...);
		}

		/// <summary>
		/// Assignment operator for nullptr. Resets the delegate to empty state.
		/// </summary>
		/// <param name="ptr"> - nullptr value.</param>
		Delegate& operator=(nullptr_t) noexcept
		{
			_concept = nullptr;
			return *this;
		}

		// <summary>
		/// Assignment operator for callable objects.
		/// </summary>
		/// <param name="func"> - The callable object to assign.</param>
		template<typename TFunc> requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate& operator=(TFunc&& func)
		{
			_concept = std::make_unique<Model<std::decay_t<TFunc>>>(std::forward<TFunc>(func));
			return *this;
		}

		/// <summary>
		/// Copy assignment operator. Copies the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to copy from.</param>
		Delegate& operator=(const Delegate& other)
		{
			_concept = other.HasTarget() ? other._concept->Copy() : nullptr;
			return *this;
		}

		/// <summary>
		/// Move assignment operator. Transfers ownership of the callable from another delegate.
		/// </summary>
		/// <param name="other"> - The delegate to move from.</param>
		Delegate& operator=(Delegate&& other) noexcept
		{
			_concept = std::exchange(other._concept, nullptr);
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return _concept != nullptr;
		}

		/// <summary>
		/// Equality operator. Compares two delegates based on their target types.
		/// </summary>
		/// <param name="left"> - The first delegate to compare.</param>
		/// <param name="right"> - The second delegate to compare.</param>
		/// <returns>True if both delegates have targets of the same type; otherwise, false.</returns>
		friend bool operator==(const Delegate& left, const Delegate& right) noexcept
		{
			bool isLeftNull = left._concept == nullptr;
			bool isRightNull = right._concept == nullptr;

			if(isLeftNull || isRightNull)
			{
				return isLeftNull && isRightNull;
			}

			return left._concept->Equals(*right._concept);
		}
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