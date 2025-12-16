#include "DelayAwaitable.h"
#include "DelayDispatcher.h"

namespace Sgl
{
	DelayAwaitable::DelayAwaitable(TimeSpan duration):
		_duration(duration)
	{}

	DelayAwaitable::DelayAwaitable(TimeSpan duration, std::stop_token stopToken):
		_duration(duration),
		_stopToken(stopToken)
	{}

	static constexpr TimeSpan ReadyDuration = TimeSpan(1e9 / 60.0);

	bool DelayAwaitable::await_ready()
	{
		return _duration < ReadyDuration;
	}

	void DelayAwaitable::await_suspend(std::coroutine_handle<> handle)
	{
		DefaultDelayDispatcher.Add(_duration, _stopToken, handle);
	}
}