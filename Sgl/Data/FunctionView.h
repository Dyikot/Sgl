#pragma once

#include "Nullable.h"

namespace Sgl
{
	template<typename T>
	class FunctionView;

	template<typename TReturn, typename... TArgs>
	class FunctionView<TReturn(TArgs...)>
	{
	public:
		template<CFunc<TReturn, TArgs...> TFunc>
		FunctionView(TFunc& function):
			_target(&function),
			_invocable(Invoke<TFunc>)
		{}

		template<CFunc<TReturn, TArgs...> TFunc>
		FunctionView(TFunc&& function):
			_target(&function),
			_invocable(Invoke<TFunc>)
		{}

		FunctionView(const FunctionView& view):
			_target(view._target),
			_invocable(view._invocable)
		{}

		FunctionView(FunctionView&& view) = delete;

		TReturn operator()(TArgs... args) const
		{
			return _invocable(_target, args...);
		}
	private:
		template<typename T>
		static TReturn Invoke(void* target, TArgs... args)
		{
			return (*static_cast<T*>(target))(args...);
		}
	private:
		void* _target;
		TReturn(*_invocable)(void*, TArgs...);
	};

	template<typename... TArgs>
	using ActionView = FunctionView<void(TArgs...)>;

	template<typename... TArgs>
	using PredicateView = FunctionView<bool(TArgs...)>;

	template<typename TResult, typename... TArgs>
	using FuncView = FunctionView<TResult(TArgs...)>;
}