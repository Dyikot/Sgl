#include "ModalWindowAwaitable.h"
#include "../../Window.h"

namespace Sgl
{
    struct WindowResumeHandler
    {
        std::coroutine_handle<> Handle;

        void operator()(Window& sender, EventArgs e) const
        {
            Handle.resume();
        }

        bool operator==(const WindowResumeHandler&) const = default;
    };

    ModalWindowAwaitable::ModalWindowAwaitable(Window& window):
        _window(window)
    {}

    ModalWindowAwaitable::~ModalWindowAwaitable()
    {
        if(_hasSuspended)
        {
            _window.Closed -= WindowResumeHandler(_handle);
        }
    }

    bool ModalWindowAwaitable::await_ready()
    {
        return !_window.IsClosed();
    }

    void ModalWindowAwaitable::await_suspend(std::coroutine_handle<> handle)
    {
        _hasSuspended = true;
        _handle = handle;
        _window.Closed += WindowResumeHandler(handle);
    }
}