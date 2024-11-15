#include "Timer.h"
#include <iostream>

using namespace std::chrono;

namespace Sgl
{
	Timer::Timer(std::chrono::milliseconds duration) noexcept:
		Duration(duration),
		SeparateThread(Duration >= 100ms),
		_timeElapsed(0ms)
	{}

	Timer::Timer(const Timer& timer) noexcept:
		Duration(timer.Duration),
		SeparateThread(timer.SeparateThread),
		_timeElapsed(timer._timeElapsed)
	{}

	Timer::~Timer() noexcept 
	{ 
		Pause();
	}

	void Timer::Start() noexcept
	{	
		if(Elapsed())
		{
			Reset();
		}
		else if(!_isPaused)
		{
			return;
		}

		_isPaused = false;

		if(SeparateThread)
		{
			auto wait = std::bind_front(&Timer::WaitOnSeparateThread, this);
			std::thread(wait).detach();
		}
		else
		{
			_start = high_resolution_clock::now();
		}
	}

	void Timer::Pause() noexcept
	{
		_isPaused = true;

		if(!SeparateThread)
		{
			_timeElapsed = duration_cast<milliseconds>(steady_clock::now() - _start);
		}
	}

	void Timer::Reset() noexcept
	{
		Pause();
		_isElapsed = false;
		_timeElapsed = 0ms;
	}

	bool Timer::Elapsed() const noexcept
	{
		if(SeparateThread)
		{
			return _isElapsed;
		}

		return _isPaused ? _timeElapsed > Duration :
						   high_resolution_clock::now() - _start + _timeElapsed > Duration;
	}

	void Timer::WaitOnSeparateThread()
	{
		std::mutex mutex;
		std::condition_variable conditionVariable;

		_start = high_resolution_clock::now();

		std::unique_lock<std::mutex> lock(mutex);
		conditionVariable.wait_for(lock, Duration - _timeElapsed,
								   std::bind_front(&Timer::IsPaused, this));

		_timeElapsed += duration_cast<milliseconds>(high_resolution_clock::now() - _start);
		_isElapsed = !_isPaused;
	}
}