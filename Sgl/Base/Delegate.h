#pragma once

#include <type_traits>
#include <memory>

namespace Sgl
{
	template<typename TInvocable, typename... TArgs>
	concept CAction = std::invocable<TInvocable, TArgs...>;

	template<typename TInvocable, typename... TArgs>
	concept CPredicate = std::is_invocable_r_v<bool, TInvocable, TArgs...>;

	template<typename TInvocable, typename TResult, typename... TArgs>
	concept CFunc = std::is_invocable_r_v<TResult, TInvocable, TArgs...>;

	template<typename T>
	class Delegate;

	template<typename TReturn, typename... TArgs>
	class Delegate<TReturn(TArgs...)>
	{
	private:
		template<typename TReturn, typename... TArgs>
		struct CallableBase
		{
		public:
			const std::type_info& type;
		public:
			CallableBase(const std::type_info& type = typeid(nullptr)):
				type(type)
			{}

			virtual ~CallableBase() = default;

			virtual TReturn operator()(TArgs... args) const = 0;
			virtual std::unique_ptr<CallableBase<TReturn, TArgs...>> Copy() const = 0;
		};

		template<CFunc<TReturn, TArgs...> TCallable>
		struct Callable: public CallableBase<TReturn, TArgs...>
		{
		private:
			TCallable _callable;
		public:
			using base = CallableBase<TReturn, TArgs...>;

			Callable(TCallable callable):
				base(typeid(TCallable)),
				_callable(std::move(callable))
			{}

			std::unique_ptr<CallableBase<TReturn, TArgs...>> Copy() const override
			{
				return std::make_unique<Callable>(*this);
			}

			TReturn operator()(TArgs... args) const override
			{
				return std::invoke(_callable, args...);
			}
		};
	public:
		Delegate() noexcept = default;

		Delegate(const Delegate& other):
			_callable(other.HasTarget() ? other._callable->Copy() : nullptr)
		{}

		Delegate(Delegate&& other) noexcept:
			_callable(std::exchange(other._callable, nullptr))
		{}

		Delegate(nullptr_t) noexcept:
			_callable(nullptr)
		{}

		template<CFunc<TReturn, TArgs...> TFunc>
		 requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate(TFunc&& func):
			_callable(std::make_unique<Callable<std::decay_t<TFunc>>>(std::forward<TFunc>(func)))
		{}

		void Reset() noexcept
		{		
			_callable.reset();
		}

		bool HasTarget() const noexcept
		{		
			return _callable != nullptr;
		}

		const std::type_info& TargetType() const noexcept
		{
			return HasTarget() ? _callable->type : typeid(nullptr);
		}

		TReturn operator()(TArgs... args) const
		{
			return (*_callable)(args...);
		}

		Delegate& operator=(nullptr_t) noexcept
		{
			_callable = nullptr;
			return *this;
		}

		template<CFunc<TReturn, TArgs...> TFunc>
			requires !std::same_as<std::decay_t<TFunc>, Delegate>
		Delegate& operator=(TFunc&& func)
		{
			_callable = std::make_unique<Callable<std::decay_t<TFunc>>>(std::forward<TFunc>(func));
			return *this;
		}

		Delegate& operator=(const Delegate& other)
		{
			if(this != &other)
			{
				_callable = other.HasTarget() ? other._callable->Copy() : nullptr;
			}

			return *this;
		}

		Delegate& operator=(Delegate&& other) noexcept
		{
			_callable = std::exchange(other._callable, nullptr);
			return *this;
		}

		friend bool operator==(const Delegate& left, const Delegate& right) noexcept
		{
			return left.TargetType() == right.TargetType();
		}
	private:
		std::unique_ptr<CallableBase<TReturn, TArgs...>> _callable;
	};

	template<typename TCallable>
	Delegate(TCallable&&) -> Delegate<std::invoke_result_t<TCallable>()>;

	template<typename... TArgs>
	using Action = Delegate<void(TArgs...)>;

	template<typename... TArgs>
	using Predicate = Delegate<bool(TArgs...)>;

	template<typename TResult, typename... TArgs>
	using Func = Delegate<TResult(TArgs...)>;
}