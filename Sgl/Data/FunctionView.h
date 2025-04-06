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
		using Callable = TReturn(*)(void*, TArgs...);

		FunctionView():
			_target(nullptr), 
			_callable(nullptr)
		{}

		template<CFunc<TReturn, TArgs...> TFunc>
		FunctionView(TFunc&& callable):
			_target(std::addressof(callable)),
			_callable(TryInvoke<TFunc>),
			_type(typeid(TFunc))
		{}

		template<CFunc<TReturn, TArgs...> TFunc>
		FunctionView(TFunc& callable):
			_target(std::addressof(callable)),
			_callable(TryInvoke<TFunc>),
			_type(typeid(TFunc))
		{}

		bool IsEmpty() const noexcept
		{
			return _target == nullptr;
		}

		const std::type_info& TargetType() const noexcept
		{
			return _type | typeid(nullptr);
		}

		TReturn Invoke(TArgs... args) const
		{
			return _callable(_target, args...);
		}

		TReturn operator()(TArgs... args) const
		{
			return _callable(_target, args...);
		}
	private:
		template<typename T>
		static TReturn TryInvoke(void* target, TArgs... args)
		{
			return (*static_cast<T*>(target))(args...);
		}
	private:
		void* _target;
		Callable _callable;
		Nullable<const std::type_info> _type;
	};
}