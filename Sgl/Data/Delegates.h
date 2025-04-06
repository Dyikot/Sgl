#pragma once

#include <type_traits>
#include <functional>

namespace Sgl
{
	template<typename TInvocable, typename... TArgs>
	concept CAction = std::invocable<TInvocable, TArgs...>;

	template<typename TInvocable, typename... TArgs>
	concept CPredicate = std::is_invocable_r_v<bool, TInvocable, TArgs...>;

	template<typename TInvocable, typename TResult, typename... TArgs>
	concept CFunc = std::is_invocable_r_v<TResult, TInvocable, TArgs...>;


	template<typename... TArgs>
	using Action = std::function<void(TArgs...)>;

	template<typename... TArgs>
	using Predicate = std::function<bool(TArgs...)>;

	template<typename TResult, typename... TArgs>
	using Func = std::function<TResult(TArgs...)>;
}