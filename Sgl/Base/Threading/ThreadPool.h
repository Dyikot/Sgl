#pragma once

#include "../Delegate.h"

namespace Sgl
{
	//! @brief Provides a thread pool for managing and executing tasks asynchronously.
	//! The thread pool maintains a collection of worker threads that can execute tasks concurrently.
	class ThreadPool
	{
	public:
		using Task = Action<>;

		//! @brief Queues a task for execution by the thread pool.
		//! The task will be executed by one of the available worker threads when possible.
		//! This method returns immediately after queuing the task.
		//! @param task The task to be executed asynchronously
		static void QueueTask(Task task);

		//! @brief Sets the maximum number of threads that the thread pool should maintain.
		//! This affects the concurrency level of the thread pool.
		//! This method should be called before using the thread pool to ensure proper initialization.
		//! @param threads The maximum number of worker threads to create in the pool
		static void SetThreadCount(size_t threads) noexcept;

		//! @brief Gets the total number of threads in the thread pool
		//! @return The total number of threads in the pool (both active and idle)
		static size_t GetThreadCount() noexcept;

		//! @brief Gets the number of tasks currently waiting in the queue to be processed
		//! @return The number of pending tasks in the queue
		static size_t GetPendingTaskCount() noexcept;
	};
}