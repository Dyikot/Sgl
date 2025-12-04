#pragma once

#include "Task.h"
#include "FuncAwaitable.h"
#include "AsyncTimeExecuter.h"

namespace Sgl
{
	class AsyncTasks
	{
	public:
		static TimeAwaitable Delay(TimeSpan duration);
		static TimeAwaitable Delay(size_t milliseconds);

		template<std::invocable TFunc>
		static auto Run(TFunc&& func)
		{
			using T = std::invoke_result_t<TFunc>;
			return FuncAwaitable<T>(std::move(func));
		}
	};
}