#include "Timer.h"
#include <iostream>

using namespace std::chrono;

namespace Sgl
{
	Timer::Timer(TimeSpan timespan) noexcept:
		Duration(timespan)
	{}

	Timer::Timer(size_t milliseconds) noexcept:
		Duration(TimeSpan::FromMilliseconds(milliseconds))
	{}

	void Timer::Start() noexcept
	{
		if(_stopwatch.IsRunning())
		{
			return;
		}

		_thread = std::jthread([this](std::stop_token stopToken)
		{
			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);
			std::condition_variable_any cv;
			nanoseconds waitDuration((Duration - _stopwatch.Elapsed()).ToNanoseconds());

			while(!stopToken.stop_requested())
			{
				_stopwatch.Start();
				cv.wait_for(lock, stopToken, waitDuration, [this] { return IsPaused(); });
				_stopwatch.Pause();

				if(IsElapsed())
				{
					Elapsed(*this);
					_stopwatch.Reset();

					if(!AutoRestart)
					{
						break;
					}
				}
			}			
		});
	}

	void Timer::Restart() noexcept
	{
		Reset();
		Start();
	}

	void Timer::Pause() noexcept
	{
		if(_thread.joinable())
		{
			_thread.request_stop();
		}

		_stopwatch.Pause();
	}

	void Timer::Reset() noexcept
	{
		if(_thread.joinable())
		{
			_thread.request_stop();
		}

		_stopwatch.Reset();
	}

	bool Timer::IsPaused() const noexcept
	{
		return !_stopwatch.IsRunning();
	}

	bool Timer::IsElapsed() const noexcept
	{
		return _stopwatch.Elapsed() >= Duration;
	}
}