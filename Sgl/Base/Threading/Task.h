#pragma once

#include <utility>
#include <coroutine>
#include <execution>
#include <type_traits>
#include <SDL3/SDL_log.h>

namespace Sgl
{
	template<typename T>
	class [[nodiscard]] Task
	{
	public:
		struct promise_type;
		using CoroutineHandle = std::coroutine_handle<promise_type>;

		struct Awaitable
		{
			CoroutineHandle Handle;

			bool await_ready() const noexcept 
			{
				return !Handle || Handle.done();
			}

			std::coroutine_handle<> await_suspend(std::coroutine_handle<> continuation)
			{
				Handle.promise().Continuation = continuation;
				return Handle;
			}

			T await_resume() const
			{
				if(auto exeption = Handle.promise().Exception)
				{
					std::rethrow_exception(exeption);
				}

				if constexpr(!std::is_void_v<T>)
				{
					return std::move(Handle.promise().Result);
				}
			}
		};		

		struct FinalAwaiter
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

		struct promise_type
		{				
			T Result {};
			std::exception_ptr Exception;
			std::coroutine_handle<> Continuation;

			Task get_return_object() 
			{
				return Task(CoroutineHandle::from_promise(*this));
			}

			std::suspend_always initial_suspend() noexcept { return {}; }
			FinalAwaiter final_suspend() noexcept {	return {}; }

			void unhandled_exception() 
			{
				Exception = std::current_exception();
			}

			void return_value(T result) noexcept
			{
				Result = std::move(result);
			}
		};
	private:
		CoroutineHandle _handle;
	public:
		explicit Task(CoroutineHandle handle):
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

		Awaitable operator co_await() noexcept
		{
			return Awaitable(_handle);
		}

		Task& operator=(Task&& other) noexcept
		{
			std::swap(_handle, other._handle);
			return *this;
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

	struct AsyncVoid
	{
		struct promise_type
		{
			AsyncVoid get_return_object() { return {}; }
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
					SDL_Log("%s", e.what());
				}
			}
			void return_void() noexcept {}
		};
	};
}