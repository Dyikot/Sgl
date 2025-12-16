#pragma once

#include <string>

#include "DelayAwaitable.h"
#include "TaskAwaitable.h"

namespace Sgl
{
	class Tasks
	{
	public:
		static DelayAwaitable Delay(TimeSpan duration);
		static DelayAwaitable Delay(TimeSpan duration, std::stop_token stopToken);
		static DelayAwaitable Delay(size_t milliseconds);
		static DelayAwaitable Delay(size_t milliseconds, std::stop_token stopToken);

		template<std::invocable TFunc, typename T = std::invoke_result_t<TFunc>>
		static TaskAwaitable<T> New(TFunc&& func, bool saveContext = false)
		{
			return TaskAwaitable<T>(std::move(func), saveContext);
		}

		static TaskAwaitable<std::string> ReadTextAsync(std::string path, bool saveContext = false);
		static TaskAwaitable<void> WriteTextAsync(std::string path, std::string text);
	};
}