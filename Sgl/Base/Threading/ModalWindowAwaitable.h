#pragma once

#include <coroutine>

namespace Sgl
{
	class Window;

	class ModalWindowAwaitable
	{
	private:
		bool _hasSuspended = false;
		Window& _window;
		std::coroutine_handle<> _handle;
	public:
		explicit ModalWindowAwaitable(Window& window);
		~ModalWindowAwaitable();

		bool await_ready();
		void await_suspend(std::coroutine_handle<> handle);
		void await_resume() {}
	};
}