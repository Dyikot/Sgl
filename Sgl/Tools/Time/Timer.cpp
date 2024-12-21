#include "Timer.h"
#include <iostream>

using namespace std::chrono;

namespace Sgl
{
	Timer::Timer(milliseconds duration) noexcept:
		Duration(duration),
		_timeElapsed(0ms)
	{}

	Timer::Timer(seconds duration) noexcept:
		Timer(duration_cast<milliseconds>(duration))
	{}

	Timer::~Timer()
	{
		Pause();
	}

	void Timer::Start() noexcept
	{
		if(_isElapsed)
		{
			Reset();
		}
		else if(!_isPaused)
		{
			return;
		}

		_isPaused = false;
		_thread = std::thread(std::bind_front(&Timer::Wait, this));
	}

	void Timer::Restart() noexcept
	{
		Reset();
		Start();
	}

	void Timer::Pause()
	{
		_isPaused = true;
		_conditionVariable.notify_one();
		_thread.join();
	}

	void Timer::Reset() noexcept
	{
		Pause();
		_isElapsed = false;
		_timeElapsed = 0ms;
	}

	void Timer::Wait()
	{
		std::mutex mutex;

		_start = high_resolution_clock::now();

		std::unique_lock<std::mutex> lock(mutex);
		_conditionVariable.wait_for(lock, Duration - _timeElapsed,
								   std::bind_front(&Timer::IsPaused, this));

		_timeElapsed += duration_cast<milliseconds>(high_resolution_clock::now() - _start);
		_isElapsed = !_isPaused;

		if(_isElapsed)
		{
			OnTimerElapsed(this, TimeElapsedEventArgs{ .Duration = Duration });
		}
	}

	void Timer::OnTimerElapsed(Timer* sender, const TimeElapsedEventArgs& e)
	{
		if(Elapsed)
		{
			Elapsed(sender, e);
		}
	}
}