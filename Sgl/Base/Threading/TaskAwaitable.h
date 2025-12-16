#pragma once

#include <atomic>
#include <coroutine>
#include <optional>
#include "ThreadPool.h"
#include "Dispatcher.h"

namespace Sgl
{
	template<typename T>
	class TaskAwaitable
	{
	private:
		Func<T> _func;
		bool _saveContext;
		std::optional<T> _result {};
		std::exception_ptr _exeption;
	public:
		explicit TaskAwaitable(Func<T> func, bool saveContext = false):
			_func(std::move(func)),
			_saveContext(saveContext)
		{}

		bool await_ready() { return false; }

		void await_suspend(std::coroutine_handle<> handle)
		{
			ThreadPool::QueueTask([this, handle]
			{
				try
				{
					_result = _func();
				}
				catch(const std::exception&)
				{
					_exeption = std::current_exception();
				}

				if(_saveContext)
				{
					UIThread.AddHandle(handle);
				}
				else if(handle)
				{
					handle.resume();
				}
			});
		}

		T await_resume()
		{ 
			if(_exeption)
			{
				std::rethrow_exception(_exeption);
			}

			return std::move(_result).value();
		}
	};

	template<>
	class TaskAwaitable<void>
	{
	private:
		Action<> _action;
		bool _saveContext;
		std::exception_ptr _exeption;
	public:
		explicit TaskAwaitable(Action<> action, bool saveContext = false):
			_action(std::move(action)),
			_saveContext(saveContext)
		{}

		bool await_ready() { return false; }

		void await_suspend(std::coroutine_handle<> handle)
		{
			ThreadPool::QueueTask([this, handle]
			{
				try
				{
					_action();
				}
				catch(const std::exception&)
				{
					_exeption = std::current_exception();
				}

				if(_saveContext)
				{
					UIThread.AddHandle(handle);
				}
				else if(handle)
				{
					handle.resume();
				}
			});
		}

		void await_resume() 
		{
			if(_exeption)
			{
				std::rethrow_exception(_exeption);
			}
		}
	};

	template<typename TFunc>
	TaskAwaitable(TFunc&&) -> TaskAwaitable<std::invoke_result_t<TFunc>>;

	template<typename TFunc>
	TaskAwaitable(TFunc&&, bool) -> TaskAwaitable<std::invoke_result_t<TFunc>>;
}