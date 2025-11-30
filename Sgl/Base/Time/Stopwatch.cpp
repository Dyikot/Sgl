#include "Stopwatch.h"

namespace Sgl
{
	Stopwatch Stopwatch::StartNew()
	{
		Stopwatch s;
		s.Start();
		return s;
	}

	void Stopwatch::Start() noexcept
	{
		if(!_isRunning)
		{
			_start = Clock::now();
			_isRunning = true;
		}
	}

	void Stopwatch::Restart() noexcept
	{
		_start = Clock::now();
		_elapsed = TimeSpan::Zero;
		_isRunning = true;
	}

	void Stopwatch::Reset() noexcept
	{
		_start = Clock::now();
		_elapsed = TimeSpan::Zero;
		_isRunning = false;
	}

	void Stopwatch::Pause() noexcept
	{
		if(_isRunning)
		{
			_elapsed += GetElapsedTime();
			_isRunning = false;
		}
	}

	bool Stopwatch::IsRunning() const noexcept
	{
		return _isRunning;
	}

	TimeSpan Stopwatch::Elapsed() const noexcept
	{
		if(_isRunning)
		{
			return _elapsed + GetElapsedTime();
		}

		return _elapsed;
	}

	TimeSpan Stopwatch::GetElapsedTime() const noexcept
	{
		auto elapsed = Clock::now() - _start;
		return TimeSpan(elapsed.count());
	}
}