#include "Time.h"

using namespace std::chrono;

namespace Sgl
{
	TimeSheduler& TimeSheduler::Current()
	{
		static TimeSheduler sheduler;
		return sheduler;
	}

	void TimeSheduler::Shedule(TimeSpan duration, std::coroutine_handle<> handle)
	{
		Shedule(duration, std::stop_token(), handle);
	}

	void TimeSheduler::Shedule(TimeSpan duration, std::stop_token stopToken, std::coroutine_handle<> handle)
	{
		auto durationNs = nanoseconds(duration.ToNanoseconds());
		auto wakeTime = high_resolution_clock::now() + durationNs;

		std::lock_guard<std::mutex> _lock(_mutex);

		if(stopToken.stop_possible())
		{
			_stopablePendings.push_back(StopablePending(wakeTime, stopToken, handle));
		}
		else
		{
			_pendings.push(Pending(wakeTime, handle));
		}
	}

	void TimeSheduler::Run()
	{
		std::vector<std::coroutine_handle<>> handlesToResume;

		{
			std::lock_guard<std::mutex> _lock(_mutex);
			auto now = high_resolution_clock::now();

			while(!_pendings.empty())
			{
				auto& pending = _pendings.top();

				if(pending.WakeTime <= now)
				{
					if(auto handle = pending.Handle)
					{
						handlesToResume.push_back(handle);
					}

					_pendings.pop();
				}
				else
				{
					break;
				}
			}

			for(auto it = _stopablePendings.begin(); it != _stopablePendings.end();)
			{
				auto& pending = *it;

				if(pending.WakeTime <= now || pending.StopToken.stop_requested())
				{
					if(auto handle = pending.Handle)
					{
						handlesToResume.push_back(handle);
					}

					it = _stopablePendings.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		
		for(auto handle : handlesToResume)
		{
			handle.resume();
		}
	}

	TimeAwaitable::TimeAwaitable(TimeSpan duration):
		_duration(duration)
	{}

	TimeAwaitable::TimeAwaitable(TimeSpan duration, std::stop_token stopToken):
		_duration(duration),
		_stopToken(stopToken)
	{}

	static constexpr TimeSpan ReadyDuration = TimeSpan(1e9 / 60.0);

	bool TimeAwaitable::await_ready()
	{
		return _duration < ReadyDuration;
	}

	void TimeAwaitable::await_suspend(std::coroutine_handle<> handle)
	{
		TimeSheduler::Current().Shedule(_duration, _stopToken, handle);
	}
}