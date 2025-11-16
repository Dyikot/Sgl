#pragma once

#include <SDL3/SDL_timer.h>
#include "TimeSpan.h"

namespace Sgl
{
	/// <summary>
	/// Block current thread for a specified duraion
	/// </summary>
	/// <param name="duration"> - sleep time duration</param>
	inline void SleepFor(TimeSpan duration)
	{
		constexpr TimeSpan _20ms = TimeSpan::FromMilliseconds(20);

		if(duration >= _20ms)
		{
			SDL_Delay(duration.ToMilliseconds());
		}
		else
		{
			SDL_DelayNS(duration.ToNanoseconds());
		}
	}
}