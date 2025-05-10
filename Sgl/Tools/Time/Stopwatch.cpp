#include "Stopwatch.h"

namespace Sgl
{
	void Stopwatch::Start()
	{
		if(!_isRunning)
		{
			_start = Clock::now();
			_isRunning = true;
		}
	}

	void Stopwatch::Restart()
	{
		_isRunning = false;
		Reset();
		Start();
	}

	void Stopwatch::Reset()
	{
		_start = Clock::now();
		_elapsed = TimeSpan::Zero();
	}

	void Stopwatch::Pause()
	{
		if(_isRunning)
		{
			_elapsed += GetEplapsedTime();
			_isRunning = false;
		}
	}

	bool Stopwatch::IsRunning() const
	{
		return _isRunning;
	}

	TimeSpan Stopwatch::Elapsed()
	{
		if(_isRunning)
		{
			_elapsed += GetEplapsedTime();
			_start = Clock::now();
		}

		return _elapsed;
	}

	TimeSpan Stopwatch::GetEplapsedTime()
	{
		auto elsapsed = Clock::now() - _start;
		return TimeSpan(elsapsed.count());
	}
}