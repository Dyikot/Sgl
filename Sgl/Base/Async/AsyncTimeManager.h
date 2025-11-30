#pragma once

#include <chrono>
#include <queue>
#include "../Time/TimeSpan.h"
#include "Task.h"

namespace Sgl
{
	class AsyncTimeManager
	{
    private:
        struct Pending
        {
            std::chrono::steady_clock::time_point ActivationTime;
            std::coroutine_handle<> Handle;

            bool operator>(const Pending& other) const
            {
                return ActivationTime > other.ActivationTime;
            }
        };

        using PendingQueue = std::priority_queue<
            Pending, 
            std::vector<Pending>, 
            std::greater<Pending>
        >;

        PendingQueue _queue;
    public:
        void Add(TimeSpan duration, std::coroutine_handle<> handle);
        void Process();
	};

    struct TimeAwaitable
    {
    private:
        TimeSpan _duration;
    public:
        explicit TimeAwaitable(TimeSpan duration);
        bool await_ready();
        void await_suspend(std::coroutine_handle<> handle);
        void await_resume() {}
    };
}