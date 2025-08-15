#include "Stopwatch.h"

namespace Sgl
{
	Stopwatch Stopwatch::StartNew()
	{
		Stopwatch s;
		s.Start();
		return s;
	}

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
		_start = Clock::now();
		_elapsed = TimeSpan::Zero;
		_isRunning = true;
	}

	void Stopwatch::Reset()
	{
		_start = Clock::now();
		_elapsed = TimeSpan::Zero;
		_isRunning = false;
	}

	void Stopwatch::Pause()
	{
		if(_isRunning)
		{
			_elapsed += GetElapsedTime();
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
			return _elapsed + GetElapsedTime();
		}

		return _elapsed;
	}

	TimeSpan Stopwatch::GetElapsedTime()
	{
		auto elapsed = Clock::now() - _start;
		return TimeSpan(elapsed.count());
	}
}