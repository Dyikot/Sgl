#include "Stopwatch.h"

namespace Sgl
{
	void Stopwatch::Start()
	{
		if(!_running)
		{
			_start = std::chrono::high_resolution_clock::now();
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
		_start = std::chrono::high_resolution_clock::now();
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

	TimeSpan Stopwatch::Elapsed()
	{
		if(_running)
		{
			_elapsed += GetEplapsedTime();
			_start = std::chrono::high_resolution_clock::now();
		}

		return _elapsed;
	}
}