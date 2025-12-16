#pragma once

#include "../Delegate.h"

namespace Sgl
{
	/// <summary>
	/// Provides a thread pool for managing and executing tasks asynchronously.
	/// The thread pool maintains a collection of worker threads that can execute tasks concurrently.
	/// </summary>
	class ThreadPool
	{
	public:
		using Task = Action<>;

		/// <summary>
		/// Queues a task for execution by the thread pool.
		/// The task will be executed by one of the available worker threads when possible.
		/// This method returns immediately after queuing the task.
		/// </summary>
		/// <param name="task">- the task to be executed asynchronously</param>
		static void QueueTask(Task task);

		/// <summary>
		/// Sets the maximum number of threads that the thread pool should maintain.
		/// This affects the concurrency level of the thread pool.
		/// This method should be called before using the thread pool to ensure proper initialization.
		/// </summary>
		/// <param name="threads">- the maximum number of worker threads to create in the pool</param>
		static void SetThreadCount(size_t threads) noexcept;

		/// <summary>
		/// Gets the total number of threads in the thread pool.
		/// </summary>
		/// <returns>The total number of threads in the pool (both active and idle)</returns>
		static size_t GetThreadCount() noexcept;

		/// <summary>
		/// Gets the number of tasks currently waiting in the queue to be processed.
		/// </summary>
		/// <returns>The number of pending tasks in the queue</returns>
		static size_t GetPendingTaskCount() noexcept;
	};
}