#pragma once

#include <vector>
#include <mutex>
#include <coroutine>
#include "../Delegate.h"

namespace Sgl
{
	class Dispatcher
	{
	private:		
		std::mutex _mutex;
		std::vector<Action<>> _tasks;
		std::vector<std::coroutine_handle<>> _handles;
	public:
		void Post(Action<> task);
		void AddHandle(std::coroutine_handle<> hanlde);
	private:
		void ProcessTasks();

		friend class Application;
	};

	inline Dispatcher UIThread;
}