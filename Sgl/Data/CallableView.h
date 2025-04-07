#pragma once

#include "Nullable.h"

namespace Sgl
{
	template<typename T>
	class CallableView;

	template<typename TReturn, typename... TArgs>
	class CallableView<TReturn(TArgs...)>
	{
	public:
		template<CFunc<TReturn, TArgs...> TFunc>
		CallableView(TFunc& function):
			_target(&function),
			_invocable(Invoke<TFunc>)
		{}

		template<CFunc<TReturn, TArgs...> TFunc>
		CallableView(TFunc&& function):
			_target(&function),
			_invocable(Invoke<TFunc>)
		{}

		CallableView(const CallableView& view):
			_target(view._target),
			_invocable(view._invocable)
		{}

		CallableView(CallableView&& view) = delete;

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
	using ActionView = CallableView<void(TArgs...)>;

	template<typename... TArgs>
	using PredicateView = CallableView<bool(TArgs...)>;

	template<typename TResult, typename... TArgs>
	using FuncView = CallableView<TResult(TArgs...)>;
}