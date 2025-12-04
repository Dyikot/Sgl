#pragma once

#include <queue>
#include "../Delegate.h"
#include <unordered_map>

namespace Sgl
{
	enum class DispatcherPriority
	{
		Input, Process, Render
	};

	class Dispatcher
	{
	private:		
		using Task = Action<>;

		std::unordered_map<DispatcherPriority, std::queue<Task>> _queues;
	public:
		void Post(Task task);
		void Post(DispatcherPriority priority, Task task);
	private:
		void Run(DispatcherPriority priority);

		friend class Application;
	};

	inline Dispatcher UIThread;
}