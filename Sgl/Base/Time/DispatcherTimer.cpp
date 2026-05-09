#include "DispatcherTimer.h"
#include "../Threading/Dispatcher.h"

namespace Sgl
{
	DispatcherTimer::DispatcherTimer(int64_t milliseconds):
		DispatcherTimer(TimeSpan::FromMilliseconds(milliseconds))
	{}

	DispatcherTimer::DispatcherTimer(TimeSpan interval):
		Interval(interval)
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

