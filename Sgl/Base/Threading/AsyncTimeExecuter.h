#pragma once

#include <chrono>
#include <queue>
#include <coroutine>
#include "../Time/TimeSpan.h"

namespace Sgl
{
	class AsyncTimeExecuter
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
        void Shedule(TimeSpan duration, std::coroutine_handle<> handle);
    private:
        void Run();

        friend class Application;
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