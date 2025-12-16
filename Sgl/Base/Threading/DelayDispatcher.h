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
	class DelayDispatcher
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
        void Add(TimeSpan duration, std::coroutine_handle<> handle);
        void Add(TimeSpan duration, std::stop_token stopToken, std::coroutine_handle<> handle);
    private:
        void Process();

        friend class Application;
	};

    inline DelayDispatcher DefaultDelayDispatcher;    
}