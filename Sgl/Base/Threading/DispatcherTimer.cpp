#include "DispatcherTimer.h"
#include "Dispatcher.h"

namespace Sgl
{
	DispatcherTimer::DispatcherTimer(TimeSpan interval, TimeSpan delay):
		Delay(delay),
		Interval(interval)
	{}

	DispatcherTimer::DispatcherTimer(int64_t intervalMilliseconds, int64_t delayMilliseconds):
		DispatcherTimer(TimeSpan::FromMilliseconds(intervalMilliseconds),
						TimeSpan::FromMilliseconds(delayMilliseconds))
	{}

	DispatcherTimer::~DispatcherTimer()
	{
		Stop();
	}

	void DispatcherTimer::Start()
	{
		if(_running)
		{
			return;
		}

		_running = true;
		UIThread.AddTimer(*this);
	}

	void DispatcherTimer::Stop()
	{
		if(!_running)
		{
			return;
		}

		_running = false;
		UIThread.RemoveTimer(*this);
	}
}

