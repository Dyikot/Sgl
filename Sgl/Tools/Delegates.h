#pragma once

#include <type_traits>

namespace Sgl
{
	template<typename TInvocable, typename TResult, typename... TArgs>
	concept Func = std::is_invocable_r_v<TResult, TInvocable, TArgs...>;

	template<typename TReturn, typename... TArgs>
	struct Function
	{
		virtual ~Function() = default;

		virtual TReturn operator()(TArgs&&... args) {}
		virtual TReturn operator()(TArgs&&... args) const {}
	};

	template<typename T>
	class Callable;

	template<typename TReturn, typename... TArgs>
	class Callable<Function<TReturn, TArgs...>>
	{
	public:
		using Function = Function<TReturn, TArgs...>;

		Callable() = default;

		template<std::derived_from<Function> TFunction>
		Callable(TFunction* function):
			_function(function)
		{}

		TReturn Invoke(TArgs&&... args)
		{
			return (*_function)(std::forward<TArgs>(args)...);
		}

		TReturn Invoke(TArgs&&... args) const
		{
			return (*_function)(std::forward<TArgs>(args)...);
		}

		TReturn operator()(TArgs&&... args)
		{
			return (*_function)(std::forward<TArgs>(args)...);
		}

		TReturn operator()(TArgs&&... args) const
		{
			return (*_function)(std::forward<TArgs>(args)...);
		}
	private:
		std::unique_ptr<Function> _function;
	};
}