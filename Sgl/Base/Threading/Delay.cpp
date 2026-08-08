#include "Delay.h"
#include "Dispatcher.h"

namespace Sgl
{
	static constexpr TimeSpan ReadyDuration = TimeSpan(1e9 / 60.0);

	DelayAwaiter::DelayAwaiter(TimeSpan duration, std::stop_token stopToken):
		_duration(duration),
		_stopToken(stopToken)
	{}

	bool DelayAwaiter::await_ready()
	{
		return _duration < ReadyDuration;
	}

	void DelayAwaiter::await_suspend(std::coroutine_handle<> handle)
	{
		UIThread.AddHandle(_duration, _stopToken, handle);
	}
}