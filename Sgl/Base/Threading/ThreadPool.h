#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <semaphore>

#include "../Delegate.h"

namespace Sgl
{
	class ThreadPool
	{
	private:
		using Task = Action<>;

		std::queue<Task> _tasks;
		std::vector<std::jthread> _workers;
		std::mutex _mutex;
		std::counting_semaphore<> _semaphore { 0 };
	public:
		static ThreadPool& Instance();

		int GetThreadCount() const noexcept;
		int GetPendingTaskCount() const noexcept;

		void QueueTask(Task task);
	private:
		ThreadPool(int maxWorkers);
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		~ThreadPool();
	};
}