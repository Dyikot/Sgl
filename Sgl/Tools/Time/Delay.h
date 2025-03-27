#pragma once

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
		std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point end = start + std::chrono::nanoseconds(duration.ToNanoseconds());

		if(duration >= TimeSpan::FromMilliseconds(15))
		{
			SDL_Delay(duration.ToMilliseconds());
		}
		else if(duration >= TimeSpan::FromMicroseconds(100))
		{
			while(std::chrono::high_resolution_clock::now() < end)
			{
				std::this_thread::yield();
			}
		}
	}
}