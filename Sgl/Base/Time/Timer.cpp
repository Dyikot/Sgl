#include "Timer.h"
#include <iostream>

using namespace std::chrono;

namespace Sgl
{
	Timer::Timer(TimeSpan timespan) noexcept:
		Duration(timespan)
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
		_thread = std::thread(&Timer::Wait, this);
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
		_stopwatch.Reset();
	}

	bool Timer::IsPaused() const noexcept
	{
		return _isPaused;
	}

	bool Timer::IsElapsed() const noexcept
	{
		return _isElapsed;
	}

	void Timer::Wait()
	{
		std::mutex mutex;

		_stopwatch.Start();
		nanoseconds waitDuration((Duration - _stopwatch.Elapsed()).ToNanoseconds());

		std::unique_lock<std::mutex> lock(mutex);
		_conditionVariable.wait_for(lock, waitDuration, [this] { return IsPaused(); });

		_stopwatch.Pause();
		_isElapsed = !_isPaused;

		if(_isElapsed)
		{
			Elapsed(*this);
		}
	}
}