#pragma once

#include "TimeSpan.h"
#include <chrono>

namespace Sgl
{
	class Stopwatch
	{
	private:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::steady_clock::time_point;

		bool _isRunning = false;
		TimeSpan _elapsed = TimeSpan::Zero();
		TimePoint _start;
	public:
		static Stopwatch StartNew();

		void Start();
		void Restart();
		void Reset();
		void Pause();
		bool IsRunning() const;
		TimeSpan Elapsed();
	private:
		TimeSpan GetEplapsedTime();
	};
}