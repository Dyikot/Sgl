#pragma once

#include "Time.h"
#include "Task.h"
#include "FuncAwaitable.h"

namespace Sgl
{
	class AsyncTasks
	{
	public:
		static TimeAwaitable Delay(TimeSpan duration);
		static TimeAwaitable Delay(TimeSpan duration, std::stop_token stopToken);
		static TimeAwaitable Delay(size_t milliseconds);
		static TimeAwaitable Delay(size_t milliseconds, std::stop_token stopToken);

		template<std::invocable TFunc, typename T = std::invoke_result_t<TFunc>>
		static FuncAwaitable<T> Run(TFunc&& func)
		{
			return FuncAwaitable<T>(std::move(func));
		}
	};
}