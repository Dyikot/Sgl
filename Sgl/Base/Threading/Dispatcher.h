#pragma once

#include <queue>
#include <unordered_map>
#include "../Delegate.h"

namespace Sgl
{
	enum class DispatcherPriority
	{
		Input, Process, Render
	};

	class Dispatcher
	{
	private:		
		std::unordered_map<DispatcherPriority, std::queue<Action<>>> _queues;
	public:
		void Post(Action<> task);
		void Post(DispatcherPriority priority, Action<> task);
	private:
		void Run(DispatcherPriority priority);

		friend class Application;
		friend class Window;
	};

	inline Dispatcher UIThread;
}