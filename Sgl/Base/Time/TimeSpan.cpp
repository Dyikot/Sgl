#include "TimeSpan.h"

namespace Sgl
{
	std::string TimeSpan::ToString() const
	{
		const auto ns = _nanoseconds;

		if(ns < ThresholdMicroseconds)
		{
			return std::format("{} ns", GetNanoseconds());
		}
		else if(ns < ThresholdMilliseconds)
		{
			return std::format("{:.2f} us", ToMicroseconds());
		}
		else if(ns < ThresholdSeconds)
		{
			return std::format("{:.2f} ms", ToMilliseconds());
		}
		else if(ns < ThresholdDays)
		{
			return std::format("{:02}:{:02}:{:02}", GetHours(), GetMinutes(), GetSeconds());
		}
		else
		{
			return std::format("{}.{:02}:{:02}:{:02}", GetDays(), GetHours(), GetMinutes(), GetSeconds());
		}
	}
}