#include "Dispatcher.h"

namespace Sgl
{
	void Dispatcher::Post(Action<> task)
	{
		std::lock_guard lock(_mutex);
		_tasks.push_back(std::move(task));
	}

	void Dispatcher::AddHandle(std::coroutine_handle<> hanlde)
	{
		std::lock_guard lock(_mutex);
		_handles.push_back(hanlde);
	}

	void Dispatcher::ProcessTasks()
	{
		std::vector<Action<>> tasks;
		std::vector<std::coroutine_handle<>> handles;

		{
			std::lock_guard lock(_mutex);
			tasks.swap(_tasks);
			handles.swap(_handles);
		}

		for(auto& task : tasks)
		{
			task();
		}

		for(auto handle : handles)
		{
			if(handle)
			{
				handle.resume();
			}
		}
	}
}

