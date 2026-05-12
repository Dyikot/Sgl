#include "Dispatcher.h"
#include "../Time/DispatcherTimer.h"

using Ns = std::chrono::nanoseconds;
using Clock = std::chrono::high_resolution_clock;

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

	void Dispatcher::AddHandle(TimeSpan duration, std::coroutine_handle<> handle)
	{
		AddHandle(duration, std::stop_token(), handle);
	}

	void Dispatcher::AddHandle(TimeSpan duration, std::stop_token stopToken, std::coroutine_handle<> handle)
	{
		auto resumeTime = Clock::now() + Ns(duration.GetNanoseconds());

		if(stopToken.stop_possible())
		{
			std::lock_guard lock(_mutex);
			_stopableDelayHandles.push_back(StopableDelayHandle(resumeTime, stopToken, handle));
		}
		else
		{
			std::lock_guard lock(_mutex);
			_delayHandles.push(DelayHandle(resumeTime, handle));
		}
	}

	void Dispatcher::AddTimer(DispatcherTimer& timer)
	{
		auto tickTime = Clock::now() + Ns(timer.Interval.GetNanoseconds());
		_timerContexts.emplace_back(&timer, tickTime);
	}

	void Dispatcher::RemoveTimer(DispatcherTimer& timer)
	{
		std::erase(_timerContexts, &timer);
	}

	void Dispatcher::Process()
	{
		std::vector<Action<>> tasks;
		std::vector<std::coroutine_handle<>> handles;

		auto now = Clock::now();

		// Processing delay coroutines
		{
			std::lock_guard lock(_mutex);
			tasks.swap(_tasks);
			handles.swap(_handles);

			while(!_delayHandles.empty())
			{
				auto& [wakeTime, handle] = _delayHandles.top();

				if(wakeTime <= now)
				{
					handles.push_back(handle);
					_delayHandles.pop();
				}
				else
				{
					break;
				}
			}

			for(auto it = _stopableDelayHandles.begin(); it != _stopableDelayHandles.end();)
			{
				auto& [wakeTime, stopToken, handle] = *it;

				if(wakeTime <= now || stopToken.stop_requested())
				{
					handles.push_back(handle);
					it = _stopableDelayHandles.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		// Coroutines handles resuming
		for(auto handle : handles)
		{
			if(handle)
			{
				handle.resume();
			}
		}

		// Timers processing
		for(auto& [timer, tickTime] : _timerContexts)
		{
			if(tickTime <= now)
			{
				timer->Tick.Invoke(*timer);
				tickTime = now + Ns(timer->Interval.GetNanoseconds());
			}
		}

		// Tasks invoking
		for(auto& task : tasks)
		{
			task();
		}		
	}
}

