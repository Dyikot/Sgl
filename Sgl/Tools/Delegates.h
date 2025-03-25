#pragma once

#include <type_traits>

namespace Sgl
{
	template<typename TInvocable, typename TResult, typename... TArgs>
	concept Func = std::is_invocable_r_v<TResult, TInvocable, TArgs...>;
}