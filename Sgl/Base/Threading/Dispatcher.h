#pragma once

#include <vector>
#include <mutex>
#include <coroutine>
#include "../Delegate.h"

namespace Sgl
{
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
	private:
		void Process();
	private:
		friend class Application;

		std::mutex _mutex;
		std::vector<Action<>> _tasks;
		std::vector<std::coroutine_handle<>> _handles;
	};

	inline Dispatcher UIThread;
}