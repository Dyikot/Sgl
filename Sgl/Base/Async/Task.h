#pragma once

#include <coroutine>

namespace Sgl
{
	struct promise;

	struct Task : std::coroutine_handle<promise>
	{
		using promise_type = Sgl::promise;
	};

	struct promise
	{
		Task get_return_object() { return {}; }
		std::suspend_never initial_suspend() noexcept { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() {}
	};
}