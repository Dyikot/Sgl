#include "Timer.h"
#include <iostream>

using namespace std::chrono;

namespace Sgl
{
	Timer::Timer(TimeSpan timespan) noexcept:
		duration(timespan)
	{}

	Timer::~Timer()
	{
		Pause();
	}

	void Timer::Start() noexcept
	{
		if(_elapsed)
		{
			Reset();
		}
		else if(!_paused)
		{
			return;
		}

		_paused = false;
		_thread = std::thread(std::bind_front(&Timer::Wait, this));
	}

	void Timer::Restart() noexcept
	{
		Reset();
		Start();
	}

	void Timer::Pause()
	{
		_paused = true;
		_conditionVariable.notify_one();
		_thread.join();
	}

	void Timer::Reset() noexcept
	{
		Pause();
		_elapsed = false;
		_stopwatch.Reset();
	}

	void Timer::Wait()
	{
		std::mutex mutex;

		_stopwatch.Start();
		nanoseconds waitDuration((duration - _stopwatch.Elapsed()).ToNanoseconds());

		std::unique_lock<std::mutex> lock(mutex);
		_conditionVariable.wait_for(lock, waitDuration,
								    std::bind_front(&Timer::IsPaused, this));

		_stopwatch.Pause();
		_elapsed = !_paused;

		if(_elapsed)
		{
			Elapsed.TryRaise(*this, TimeElapsedEventArgs{.duration = duration });
		}
	}
}