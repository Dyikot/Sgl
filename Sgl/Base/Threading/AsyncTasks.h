#pragma once

#include <string>

#include "Time.h"
#include "Task.h"
#include "TaskAwaitable.h"

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
		static TaskAwaitable<T> New(TFunc&& func, bool saveContext = false)
		{
			return TaskAwaitable<T>(std::move(func), saveContext);
		}

		static TaskAwaitable<std::string> ReadText(std::string path, bool saveContext = false);
		static TaskAwaitable<void> WriteText(std::string path, std::string text);
	};
}