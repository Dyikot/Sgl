#include "Timer.h"
#include <iostream>

using namespace std::chrono;

namespace Sgl
{
	Timer::Timer(TimeSpan timespan) noexcept:
		_duration(milliseconds(static_cast<int>(timespan.Milliseconds()))),
		_timeElapsed(0ms)
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
		_timeElapsed = 0ms;
	}

	void Timer::Wait()
	{
		std::mutex mutex;

		_start = high_resolution_clock::now();

		std::unique_lock<std::mutex> lock(mutex);
		_conditionVariable.wait_for(lock, _duration - _timeElapsed,
								   std::bind_front(&Timer::IsPaused, this));

		_timeElapsed += duration_cast<milliseconds>(high_resolution_clock::now() - _start);
		_elapsed = !_paused;

		if(_elapsed)
		{
			Elapsed.TryInvoke(this, TimeElapsedEventArgs{.Duration = TimeSpan::FromMilliseconds(_duration.count()) });
		}
	}
}