#include "TimeSpan.h"

namespace Sgl
{
	std::string TimeSpan::ToString() const
	{
		const auto ns = _nanoseconds;

		if(ns < ThresholdMicroseconds)
		{
			return std::format("{} ns", ToNanoseconds());
		}
		else if(ns < ThresholdMilliseconds)
		{
			return std::format("{:.2f} us", ToMicroseconds());
		}
		else if(ns < ThresholdSeconds)
		{
			return std::format("{:.2f} ms", ToMilliseconds());
		}
		else if(ns < ThresholdMinutes)
		{
			return std::format("{:.2f} sec", ToSeconds());
		}
		else if(ns < ThresholdHours)
		{
			return std::format("{:.2f} min", ToMinutes());
		}
		else if(ns < ThresholdDays)
		{
			return std::format("{:.2f} hr", ToHours());
		}
		else
		{
			return std::format("{:.2f} days", ToDays());
		}
	}
}