#pragma once

#include "Delegates.h"

namespace Sgl
{
	template<typename T>
	class CallableView;

	template<typename TReturn, typename... TArgs>
	class CallableView<TReturn(TArgs...)>
	{
	public:
		template<CFunc<TReturn, TArgs...> TFunc>
		CallableView(TFunc&& callable):
			_callable(&callable),
			_erased(OnErasedCall<std::remove_reference_t<TFunc>>)
		{}

		CallableView(const CallableView& view):
			_callable(view._callable),
			_erased(view._erased)
		{}

		CallableView(CallableView&& view) = delete;

		TReturn operator()(TArgs... args) const
		{
			return _erased(_callable, std::forward<TArgs>(args)...);
		}

		template<CFunc<TReturn, TArgs...> TFunc>
		CallableView& operator=(TFunc&& callable)
		{
			_callable = &callable;
			_erased = OnErasedCall<std::remove_reference_t<TFunc>>;
			return *this;
		}

		CallableView& operator=(const CallableView& view)
		{
			_callable = view._callable;
			_erased = view._erased;
			return *this;
		}
	private:
		template<typename T>
		static TReturn OnErasedCall(void* callable, TArgs... args)
		{
			return (*static_cast<T*>(callable))(std::forward<TArgs>(args)...);
		}
	private:
		void* _callable;
		TReturn(*_erased)(void*, TArgs...);
	};

	template<typename TReturn, typename... TArgs>
	class CallableView<TReturn(TArgs...) const>
	{
	public:
		template<CFunc<TReturn, TArgs...> TFunc>
		CallableView(const TFunc& callable):
			_callable(&callable),
			_erased(OnErasedCall<std::remove_reference_t<TFunc>>)
		{}

		CallableView(const CallableView& view):
			_callable(view._callable),
			_erased(view._erased)
		{}

		CallableView(CallableView&& view) = delete;

		TReturn operator()(TArgs... args) const
		{
			return _erased(_callable, std::forward<TArgs>(args)...);
		}

		template<CFunc<TReturn, TArgs...> TFunc>
		CallableView& operator=(TFunc&& callable)
		{
			_callable = &callable;
			_erased = OnErasedCall<std::remove_reference_t<TFunc>>;
			return *this;
		}

		CallableView& operator=(const CallableView& view)
		{
			_callable = view._callable;
			_erased = view._erased;
			return *this;
		}
	private:
		template<typename T>
		static TReturn OnErasedCall(const void* callable, TArgs... args)
		{
			return (*static_cast<const T*>(callable))(std::forward<TArgs>(args)...);
		}
	private:
		const void* _callable;
		TReturn(*_erased)(const void*, TArgs...);
	};

	template<typename TCallable>
	CallableView(TCallable&&) -> CallableView<std::invoke_result_t<TCallable>()>;

	template<typename TCallable>
	CallableView(const TCallable&) -> CallableView<std::invoke_result_t<TCallable>() const>;

	template<typename... TArgs>
	using ActionView = CallableView<void(TArgs...)>;

	template<typename... TArgs>
	using PredicateView = CallableView<bool(TArgs...)>;

	template<typename TResult, typename... TArgs>
	using FuncView = CallableView<TResult(TArgs...)>;
}