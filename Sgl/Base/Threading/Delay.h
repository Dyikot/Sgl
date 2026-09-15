#pragma once

#include <stop_token>
#include <coroutine>
#include "../Time/TimeSpan.h"

namespace Sgl
{
    //! @brief An awaiter object that suspends a coroutine for a specified duration, enabling asynchronous time delays.
    //! 
    //! The delay is managed by the DelayDispatcher.
    //! Supports optional cancellation via std::stop_token. When awaited, the coroutine resumes on the main thread
    //! after the delay elapses or immediately if canceled.
    class DelayAwaiter
    {
    public:
        DelayAwaiter(TimeSpan delay, std::stop_token stopToken);

        bool await_ready();
        void await_suspend(std::coroutine_handle<> handle);
        void await_resume() {}

    private:
        TimeSpan _duration;
        std::stop_token _stopToken;
    };

    //! @brief Creates an awaiter that suspends the coroutine for the specified duration
    //! @param delay The time span to wait before resuming
    //! @param stopToken A token that can be used to cancel the delay
    //! @return A DelayAwaiter that completes after the specified delay
    inline auto Delay(TimeSpan delay, std::stop_token stopToken = {})
    {
        return DelayAwaiter(delay, stopToken);
    }

    //! @brief Creates an awaiter that suspends the coroutine for the specified number of milliseconds
    //! @param millisecondsDelay The duration, in milliseconds, to wait before resuming
    //! @param stopToken A token that can be used to cancel the delay
    //! @return A DelayAwaiter that completes after the specified delay
    inline auto Delay(int64_t millisecondsDelay, std::stop_token stopToken = {})
    {
        return DelayAwaiter(TimeSpan::FromMilliseconds(millisecondsDelay), stopToken);
    }
}