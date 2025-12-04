#pragma once

#include <optional>
#include <coroutine>
#include "ThreadPool.h"

namespace Sgl
{
	template<typename T>
	class FuncAwaitable
	{
	private:
		Func<T> _func;
		std::optional<T> _result;
	public:
		explicit FuncAwaitable(Func<T>&& func):
			_func(std::move(func))
		{}

		bool await_ready() { return false; }

		void await_suspend(std::coroutine_handle<> handle)
		{
			auto& threadPool = ThreadPool::Instance();
			threadPool.QueueTask([handle, this]
			{
				_result = _func();
				handle.resume();
			});
		}

		T await_resume() { return std::move(_result.value()); }
	};

	template<>
	class FuncAwaitable<void>
	{
	private:
		Action<> _action;
	public:
		explicit FuncAwaitable(Action<> action):
			_action(std::move(action))
		{}

		bool await_ready() { return false; }

		void await_suspend(std::coroutine_handle<> handle)
		{
			auto& threadPool = ThreadPool::Instance();
			threadPool.QueueTask([handle, this]
			{
				_action();
				handle.resume();
			});
		}

		void await_resume() {}
	};
}