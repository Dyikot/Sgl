#pragma once

#include <SDL/SDL_timer.h>
#include "TimeSpan.h"
#include <chrono>
#include <thread>

namespace Sgl
{
	/// <summary>
	/// Block current thread for a minimum 100 us.
	/// </summary>
	/// <param name="duration"> - sleep time duration</param>
	inline void SleepFor(TimeSpan duration)
	{
		constexpr auto millisecondsThreshold = TimeSpan::FromMilliseconds(15);
		constexpr auto microsecondsThreshold = TimeSpan::FromMicroseconds(100);

		auto start = std::chrono::high_resolution_clock::now();
		auto end = start + std::chrono::nanoseconds(duration.ToNanoseconds());

		if(duration >= millisecondsThreshold)
		{
			SDL_Delay(duration.ToMilliseconds());
		}
		else if(duration >= microsecondsThreshold)
		{
			while(std::chrono::high_resolution_clock::now() < end)
			{
				std::this_thread::yield();
			}
		}
	}
}