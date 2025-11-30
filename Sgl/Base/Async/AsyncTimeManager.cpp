#include "AsyncTimeManager.h"
#include "../../Application.h"

using namespace std::chrono;

namespace Sgl
{
	void AsyncTimeManager::Add(TimeSpan duration, std::coroutine_handle<> handle)
	{
		auto durationNs = nanoseconds(duration.ToNanoseconds());
		auto wakeTime = high_resolution_clock::now() + durationNs;
		_queue.push(Pending(wakeTime, handle));
	}

	void AsyncTimeManager::Process()
	{
		auto now = high_resolution_clock::now();

		while(!_queue.empty())
		{
			auto& next = _queue.top();

			if(next.ActivationTime <= now)
			{
				if(next.Handle)
				{
					next.Handle.resume();
				}

				_queue.pop();
			}
			else
			{
				break;
			}
		}
	}

	TimeAwaitable::TimeAwaitable(TimeSpan duration):
		_duration(duration)
	{}

	static constexpr TimeSpan ReadyDuration = TimeSpan::FromMilliseconds(15);

	bool TimeAwaitable::await_ready()
	{
		return _duration <= ReadyDuration;
	}

	void TimeAwaitable::await_suspend(std::coroutine_handle<> handle)
	{
		App->_asyncTimeManager.Add(_duration, handle);
	}
}