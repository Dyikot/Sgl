#pragma once

#include <stop_token>
#include <coroutine>

#include "../Time/TimeSpan.h"

namespace Sgl
{
    class DelayAwaitable
    {
    private:
        TimeSpan _duration;
        std::stop_token _stopToken;
    public:
        explicit DelayAwaitable(TimeSpan duration);
        DelayAwaitable(TimeSpan duration, std::stop_token stopToken);
        bool await_ready();
        void await_suspend(std::coroutine_handle<> handle);
        void await_resume() {}
    };
}