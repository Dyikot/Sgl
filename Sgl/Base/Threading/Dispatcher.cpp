#include "Dispatcher.h"
#include <stdexcept>


namespace Sgl
{
	void Dispatcher::Post(Task task)
	{
		Post(DispatcherPriority::Process, std::move(task));
	}

	void Dispatcher::Post(DispatcherPriority priority, Task task)
	{
		switch(priority)
		{
			case Sgl::DispatcherPriority::Input:
			case Sgl::DispatcherPriority::Process:
			case Sgl::DispatcherPriority::Render:
				_queues[priority].push(std::move(task));
				break;

			default:
				throw std::invalid_argument("Selected priority does not exist");
		}
	}

	void Dispatcher::Run(DispatcherPriority priority)
	{
		auto& queue = _queues[priority];

		while(!queue.empty())
		{
			auto& task = queue.front();
			task();
			queue.pop();
		}
	}
}

