#pragma once

#include <vector>
#include <queue>
#include <list>
#include <mutex>
#include <coroutine>
#include <chrono>

#include "../Delegate.h"
#include "../Time/TimeSpan.h"

namespace Sgl
{
	class DispatcherTimer;

	/// <summary>
	/// Provides a mechanism to schedule and invoke tasks on the main thread (UI thread).
	/// Ensures thread-safe execution of code that must run in the main thread context,
	/// such as UI updates.
	/// </summary>
	class Dispatcher
	{
	public:
		/// <summary>
		/// Posts a task to be executed on the main thread.
		/// The provided task is queued and will be invoked asynchronously during the main thread's message loop.
		/// This method is thread-safe and can be called from any thread.
		/// </summary>
		/// <param name="task"> - the action to execute on the main thread.</param>
		void Post(Action<> task);

		/// <summary>
		/// Adds a coroutine handle to be resumed on the main thread.
		/// This method is used internally by awaitable types to ensure that after an asynchronous operation
		/// completes, the suspended coroutine is continued on the main (UI) thread.
		/// The coroutine will be resumed during the main thread's message loop processing.
		/// </summary>
		/// <param name="handle"> - the coroutine handle to resume on the main thread.</param>
		void AddHandle(std::coroutine_handle<> hanlde);

		/// <summary>
		/// Schedules a coroutine to resume after the specified duration.
		/// The resumption will be processed on the main thread.
		/// </summary>
		/// <param name="duration"> - the delay before resuming the coroutine.</param>
		/// <param name="handle"> - the coroutine handle to resume.</param>
		void AddHandle(TimeSpan duration, std::coroutine_handle<> handle);

		/// <summary>
		/// Schedules a cancellable coroutine to resume after the specified duration.
		/// If the associated stop_token is triggered before the delay elapses, the coroutine will not resume.
		/// Resumption (if not canceled) occurs on the main thread.
		/// </summary>
		/// <param name="duration"> - the delay before resuming the coroutine.</param>
		/// <param name="stopToken"> - token used to cancel the delay.</param>
		/// <param name="handle"> - the coroutine handle to resume.</param>
		void AddHandle(TimeSpan duration, std::stop_token stopToken, std::coroutine_handle<> handle);

		/// <summary>
		/// Registers a DispatcherTimer with the dispatcher to begin managing its tick events.
		/// </summary>
		void AddTimer(DispatcherTimer& timer);

		/// <summary>
		/// Unregisters a DispatcherTimer from the dispatcher, stopping its tick events.
		/// </summary>
		void RemoveTimer(DispatcherTimer& timer);
	private:
		void Process();
	private:
		using TimePoint = std::chrono::steady_clock::time_point;		

		struct DelayHandle
		{
			TimePoint ResumeTime;
			std::coroutine_handle<> Handle;

			bool operator>(const DelayHandle& other) const
			{
				return ResumeTime > other.ResumeTime;
			}
		};

		struct StopableDelayHandle
		{
			TimePoint ResumeTime;
			std::stop_token StopToken;
			std::coroutine_handle<> Handle;
		};

		struct TimerContext
		{
			DispatcherTimer* Timer;
			TimePoint TickTime;

			bool operator==(const DispatcherTimer* timer) const
			{
				return Timer == timer;
			}
		};

		std::mutex _mutex;
		std::vector<Action<>> _tasks;
		std::vector<std::coroutine_handle<>> _handles;
		std::priority_queue<DelayHandle, std::vector<DelayHandle>, std::greater<DelayHandle>> _delayHandles;
		std::list<StopableDelayHandle> _stopableDelayHandles;
		std::vector<TimerContext> _timerContexts;

		friend class Application;
	};

	inline Dispatcher UIThread;
}