#pragma once

#include <coroutine>

namespace Sgl
{
	class Window;

	/// <summary>
	/// An awaitable object that allows a coroutine to suspend execution until a modal window is closed.
	/// </summary>
	class ModalWindowAwaitable
	{
	public:
		explicit ModalWindowAwaitable(Window& window);
		~ModalWindowAwaitable();

		bool await_ready();
		void await_suspend(std::coroutine_handle<> handle);
		void await_resume() {}
	private:
		Window& _window;
		std::coroutine_handle<> _handle;
		bool _hasSuspended = false;
	};
}