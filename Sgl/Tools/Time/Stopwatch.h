#pragma once
#include <SDL/SDL_timer.h>
#include "TimeSpan.h"

namespace Sgl
{
	class Stopwatch
	{
	private:
		size_t _start = 0;
	public:
		void Restart() { _start = SDL_GetPerformanceCounter(); }

		TimeSpan Elapsed() const
		{
			return TimeSpan::FromMilliseconds(1000.f * static_cast<float>(SDL_GetPerformanceCounter() - _start) 
											  / SDL_GetPerformanceFrequency());
		}

		TimeSpan GetElapsedAndRestart()
		{
			auto elapsed = Elapsed();
			Restart();
			return elapsed;
		}
	};
}