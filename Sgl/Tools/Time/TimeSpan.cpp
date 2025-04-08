#include "TimeSpan.h"

namespace Sgl
{
	std::string TimeSpan::ToString() const
	{
		if(ToMicroseconds() < 10)
		{
			return std::format("{} ns", ToNanoseconds());
		}
		else if(ToMilliseconds() < 10)
		{
			return std::format("{:.2f} us", ToMicroseconds());
		}
		else if(ToSeconds() < 10)
		{
			return std::format("{:.2f} ms", ToMilliseconds());
		}
		else if(ToMinutes() < 10)
		{
			return std::format("{:.2f} sec", ToSeconds());
		}
		else if(ToHours() < 10)
		{
			return std::format("{:.2f} min", ToMinutes());
		}
		else if(ToDays() < 10)
		{
			return std::format("{:.2f} hr", ToHours());
		}
		else
		{
			return std::format("{:.2f} hr", ToDays());
		}
	}
}