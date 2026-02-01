#pragma once

#include <stop_token>
#include <coroutine>

#include "../Time/TimeSpan.h"

namespace Sgl
{
    /// <summary>
    /// An awaitable object that suspends a coroutine for a specified duration, enabling asynchronous time delays.
    /// The delay is managed by the DelayDispatcher.
    /// Supports optional cancellation via std::stop_token. When awaited, the coroutine resumes on the main thread
    /// after the delay elapses or immediately if canceled.
    /// </summary>
    class DelayAwaitable
    {
    public:
        explicit DelayAwaitable(int64_t millisecondsDelay);
        explicit DelayAwaitable(TimeSpan delay);
        DelayAwaitable(int64_t millisecondsDelay, std::stop_token stopToken);
        DelayAwaitable(TimeSpan delay, std::stop_token stopToken);

        bool await_ready();
        void await_suspend(std::coroutine_handle<> handle);
        void await_resume() {}
    private:
        TimeSpan _duration;
        std::stop_token _stopToken;
    };
}