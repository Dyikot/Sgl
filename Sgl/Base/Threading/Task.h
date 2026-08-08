#pragma once

#include <coroutine>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include "../Logging.h"

namespace Sgl
{
	/// <summary>
	/// Represents an awaitable asynchronous operation that eventually produces a result of type T.
	/// Designed to be used with C++20 coroutines, where a function returning Task<T> can use co_await and co_return.
	/// </summary>
	template<typename T>
	class [[nodiscard]] Task
	{
	public:
		struct Awaiter;
		struct FinalAwaiter;
		struct promise_type;
		using CoroutineHandle = std::coroutine_handle<promise_type>;
	public:
		explicit Task(CoroutineHandle handle) noexcept:
			_handle(handle)
		{}

		Task(const Task&) = delete;

		Task(Task&& other) noexcept:
			_handle(std::exchange(other._handle, {}))
		{}

		~Task()
		{
			if(_handle)
			{
				_handle.destroy();
			}
		}

		bool IsValid() const noexcept
		{
			return static_cast<bool>(_handle);
		}

		bool IsDone() const noexcept
		{
			return _handle && _handle.done();
		}

		bool IsFaulted() const noexcept
		{
			return _handle
				&& _handle.done()
				&& _handle.promise().Exception;
		}

		bool IsSuccessful() const noexcept
		{
			return _handle
				&& _handle.done()
				&& !_handle.promise().Exception;
		}

		std::exception_ptr GetException() const noexcept
		{
			return _handle ? _handle.promise().Exception : nullptr;
		}

		T Result()
		{
			if(!_handle)
			{
				throw std::runtime_error("Task is empty");
			}

			if(!_handle.done())
			{
				throw std::logic_error("Task is not completed");
			}

			if(auto exception = _handle.promise().Exception)
			{
				std::rethrow_exception(exception);
			}

			return _handle.promise().Result;
		}

		Awaiter operator co_await() noexcept
		{
			return Awaiter(_handle);
		}

		explicit operator bool() const noexcept
		{
			return IsValid();
		}

		Task& operator=(const Task&) = delete;

		Task& operator=(Task&& other) noexcept
		{
			if(this != &other)
			{
				std::swap(_handle, other._handle);
			}

			return *this;
		}
	private:
		CoroutineHandle _handle;
	};

	template<typename T>
	struct Task<T>::Awaiter
	{
		CoroutineHandle Handle;

		bool await_ready() const noexcept
		{
			return !Handle || Handle.done();
		}

		std::coroutine_handle<> await_suspend(std::coroutine_handle<> continuation) noexcept
		{
			Handle.promise().Continuation = continuation;
			return Handle;
		}

		T await_resume()
		{
			if(!Handle)
			{
				throw std::runtime_error("Awaited empty Task");
			}

			if(auto exception = Handle.promise().Exception)
			{
				std::rethrow_exception(exception);
			}

			if constexpr(!std::is_void_v<T>)
			{
				return std::move(Handle.promise().Result);
			}
		}
	};

	template<typename T>
	struct Task<T>::FinalAwaiter
	{
		bool await_ready() const noexcept { return false; }

		std::coroutine_handle<> await_suspend(CoroutineHandle handle) noexcept
		{
			if(auto continuation = handle.promise().Continuation)
			{
				return continuation;
			}

			return std::noop_coroutine();
		}

		void await_resume() const noexcept {}
	};

	template<typename T>
	struct Task<T>::promise_type
	{
		T Result {};
		std::exception_ptr Exception;
		std::coroutine_handle<> Continuation;

		Task get_return_object()
		{
			return Task(CoroutineHandle::from_promise(*this));
		}

		std::suspend_always initial_suspend() noexcept { return {}; }
		FinalAwaiter final_suspend() noexcept { return {}; }

		void unhandled_exception()
		{
			Exception = std::current_exception();
		}

		void return_value(T result) noexcept(std::is_nothrow_move_assignable_v<T>)
		{
			Result = std::move(result);
		}
	};

	template<>
	struct [[nodiscard]] Task<void>::promise_type
	{
		std::exception_ptr Exception;
		std::coroutine_handle<> Continuation;

		Task<void> get_return_object()
		{
			return Task<void>(CoroutineHandle::from_promise(*this));
		}

		std::suspend_always initial_suspend() noexcept { return {}; }
		Task<void>::FinalAwaiter final_suspend() noexcept { return {}; }

		void unhandled_exception()
		{
			Exception = std::current_exception();
		}

		void return_void() noexcept {}
	};

	struct AsyncTask
	{
		struct promise_type
		{
			AsyncTask get_return_object() { return {}; }
			std::suspend_never initial_suspend() noexcept { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void unhandled_exception() 
			{ 
				try
				{
					std::rethrow_exception(std::current_exception());				
				}
				catch(const std::exception& e)
				{
					Logging::LogError("{}", e.what());
				}
				catch(...)
				{
					Logging::LogError("Unknown exception escaped from AsyncTask");
				}
			}
			void return_void() noexcept {}
		};
	};
}