#pragma once

#include <type_traits>
#include <functional>

namespace Sgl
{
	template<typename TInvocable, typename TResult, typename... TArgs>
	concept CFunc = std::is_invocable_r_v<TResult, TInvocable, TArgs...>;

	template<typename T>
	class Delegate;

	template<typename TReturn, typename... TArgs>
	class Delegate<TReturn(TArgs...)>
	{
	private:
		template<typename TReturn, typename... TArgs>
		struct ICallable
		{
			virtual ~ICallable() = default;

			virtual const std::type_info& TargetType() const = 0;
			virtual TReturn operator()(TArgs... args) = 0;
			virtual TReturn operator()(TArgs... args) const = 0;
		};

		template<CFunc<TReturn, TArgs...> TCallable>
		struct Callable: public ICallable<TReturn, TArgs...>
		{
		private:
			TCallable _callable;
		public:
			Callable(TCallable callable):
				_callable(std::move(callable))
			{}

			const std::type_info& TargetType() const
			{
				return typeid(TCallable);
			}

			TReturn operator()(TArgs... args) override
			{
				return std::invoke(_callable, args...);
			}

			TReturn operator()(TArgs... args) const override
			{
				return std::invoke(_callable, args...);
			}
		};
	public:
		Delegate() = default;

		template<CFunc<TReturn, TArgs...> TFunc>
		Delegate(TFunc&& callable):
			_callable(std::make_unique<Callable<TFunc>>(std::forward<TFunc>(callable)))
		{}

		void Reset()
		{
			_callable.reset();
		}

		bool IsEmpty() const noexcept
		{
			return !_callable.operator bool();
		}

		const std::type_info& TargetType() const noexcept
		{
			return _callable->TargetType();
		}

		TReturn Invoke(TArgs... args)
		{
			return (*_callable)(args...);
		}

		TReturn Invoke(TArgs... args) const
		{
			return (*_callable)(args...);
		}

		TReturn operator()(TArgs... args)
		{
			return (*_callable)(args...);
		}

		TReturn operator()(TArgs... args) const
		{
			return (*_callable)(args...);
		}
	private:
		std::unique_ptr<ICallable<TReturn, TArgs...>> _callable;
	};

	template<typename... TArgs>
	using Action = std::function<void(TArgs...)>;

	template<typename... TArgs>
	using Predicate = std::function<bool(TArgs...)>;

	template<typename TResult, typename... TArgs>
	using Func = std::function<TResult(TArgs...)>;
}