#pragma once

#include "TimeSpan.h"
#include <chrono>

namespace Sgl
{
	class Stopwatch
	{
	private:
		bool _running = false;
		TimeSpan _elapsed = TimeSpan::Zero();
		std::chrono::steady_clock::time_point _start;
	public:
		void Start()
		{
			if(!_running)
			{
				_start = std::chrono::high_resolution_clock::now();
				_running = true;
			}
		}

		void Restart()
		{
			_running = false;
			Reset();
			Start();
		}

		void Reset()
		{
			_start = std::chrono::high_resolution_clock::now();
			_elapsed = TimeSpan::Zero();
		}

		void Pause()
		{
			if(_running)
			{
				_elapsed += GetEplapsedTime();
				_running = false;
			}
		}

		bool IsRunning() const
		{
			return _running;
		}

		TimeSpan Elapsed()
		{
			if(_running)
			{
				_elapsed += GetEplapsedTime();
				_start = std::chrono::high_resolution_clock::now();
			}

			return _elapsed;
		}
	private:
		TimeSpan GetEplapsedTime()
		{
			return TimeSpan((std::chrono::high_resolution_clock::now() - _start).count());
		}
	};
}