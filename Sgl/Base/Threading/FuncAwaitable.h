#pragma once

#include <atomic>
#include <coroutine>
#include "ThreadPool.h"

namespace Sgl
{
	template<typename T>
	class FuncAwaitable
	{
	private:
		T _result {};
		std::atomic<bool> _isReady = false;
		std::coroutine_handle<> _handle;
	public:
		explicit FuncAwaitable(Func<T> func)
		{
			ThreadPool::Current().QueueTask([this, func = std::move(func)]
			{
				_result = func();				
				_isReady.store(true, std::memory_order_release);

				if(_handle)
				{
					_handle.resume();
				}
			});
		}

		bool await_ready() { return _isReady.load(std::memory_order_acquire); }

		void await_suspend(std::coroutine_handle<> handle)
		{
			_handle = handle;

			if(_isReady.load(std::memory_order_acquire))
			{
				_handle.resume();
				_handle = nullptr;
			}
		}

		T await_resume() { return std::move(_result); }
	};

	template<>
	class FuncAwaitable<void>
	{
	private:
		std::atomic<bool> _isReady = false;
		std::coroutine_handle<> _handle;
	public:
		explicit FuncAwaitable(Action<> action)
		{
			ThreadPool::Current().QueueTask([this, action = std::move(action)]
			{
				action();
				_isReady.store(true, std::memory_order_release);

				if(_handle)
				{
					_handle.resume();
				}
			});
		}

		bool await_ready() { return _isReady.load(std::memory_order_acquire); }

		void await_suspend(std::coroutine_handle<> handle)
		{
			_handle = handle;

			if(_isReady.load(std::memory_order_acquire) && handle)
			{
				_handle.resume();
				_handle = nullptr;
			}
		}

		void await_resume() {}
	};
}