#pragma once

#include <list>
#include <queue>
#include <mutex>
#include <chrono>
#include <coroutine>
#include <stop_token>

#include "../Time/TimeSpan.h"

namespace Sgl
{
	class TimeSheduler
	{
    private:
        struct Pending
        {
            std::chrono::steady_clock::time_point WakeTime;
            std::coroutine_handle<> Handle;

            bool operator>(const Pending& other) const
            {
                return WakeTime > other.WakeTime;
            }
        };

        struct StopablePending
        {
            std::chrono::steady_clock::time_point WakeTime;
            std::stop_token StopToken;
            std::coroutine_handle<> Handle;
        };

        using PendingQueue = std::priority_queue<
            Pending,
            std::vector<Pending>,
            std::greater<Pending>
        >;

        std::mutex _mutex;
        PendingQueue _pendings;
        std::list<StopablePending> _stopablePendings;
    public:
        static TimeSheduler& Current();

        void Shedule(TimeSpan duration, std::coroutine_handle<> handle);
        void Shedule(TimeSpan duration, std::stop_token stopToken, std::coroutine_handle<> handle);
    private:
        TimeSheduler() = default;
        void Run();

        friend class Application;
	};

    struct TimeAwaitable
    {
    private:
        TimeSpan _duration;
        std::stop_token _stopToken;        
    public:
        explicit TimeAwaitable(TimeSpan duration);
        TimeAwaitable(TimeSpan duration, std::stop_token stopToken);
        bool await_ready();
        void await_suspend(std::coroutine_handle<> handle);
        void await_resume() {}
    };
}