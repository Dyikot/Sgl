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
		void Start();
		void Restart();
		void Reset();
		void Pause();
		bool IsRunning() const { return _running; }
		TimeSpan Elapsed();
	private:
		TimeSpan GetEplapsedTime()
		{
			return TimeSpan((std::chrono::high_resolution_clock::now() - _start).count());
		}
	};
}