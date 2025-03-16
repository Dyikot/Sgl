#include "Stopwatch.h"

using namespace std::chrono;

namespace Sgl
{
	void Stopwatch::Start()
	{
		if(!_running)
		{
			_start = high_resolution_clock::now();
			_running = true;
		}
	}

	void Stopwatch::Restart()
	{
		_running = false;
		Reset();
		Start();
	}

	void Stopwatch::Reset()
	{
		_start = high_resolution_clock::now();
		_elapsed = TimeSpan::Zero();
	}

	void Stopwatch::Pause()
	{
		if(_running)
		{
			_elapsed += GetEplapsedTime();
			_running = false;
		}
	}

	bool Stopwatch::IsRunning() const
	{
		return _running;
	}

	TimeSpan Stopwatch::Elapsed()
	{
		if(_running)
		{
			_elapsed += GetEplapsedTime();
			_start = high_resolution_clock::now();
		}

		return _elapsed;
	}

	TimeSpan Stopwatch::GetEplapsedTime()
	{
		return TimeSpan((high_resolution_clock::now() - _start).count());
	}
}
