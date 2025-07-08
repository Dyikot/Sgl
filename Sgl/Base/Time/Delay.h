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
		auto start = std::chrono::high_resolution_clock::now();
		auto end = start + std::chrono::nanoseconds(duration.ToNanoseconds());

		if(duration >= TimeSpan::FromMilliseconds(15i64))
		{
			SDL_Delay(duration.ToMilliseconds());
		}
		else if(duration >= TimeSpan::FromMicroseconds(100i64))
		{
			while(std::chrono::high_resolution_clock::now() < end)
			{
				std::this_thread::yield();
			}
		}
	}
}