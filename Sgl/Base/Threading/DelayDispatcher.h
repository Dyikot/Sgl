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
    /// <summary>
    /// Manages time-based coroutine resumptions on the main thread by scheduling delayed continuations.
    /// Delays are processed during the main message loop, ensuring all resumptions occur on the main thread.
    /// This class is intended for internal use by DelayAwaitable and is integrated with the application's event loop.
    /// </summary>
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
    public:
        /// <summary>
        /// Schedules a coroutine to resume after the specified duration.
        /// The resumption will be processed on the main thread.
        /// </summary>
        /// <param name="duration"> - the delay before resuming the coroutine.</param>
        /// <param name="handle"> - the coroutine handle to resume.</param>
        void Add(TimeSpan duration, std::coroutine_handle<> handle);

        /// <summary>
        /// Schedules a cancellable coroutine to resume after the specified duration.
        /// If the associated stop_token is triggered before the delay elapses, the coroutine will not resume.
        /// Resumption (if not canceled) occurs on the main thread.
        /// </summary>
        /// <param name="duration"> - the delay before resuming the coroutine.</param>
        /// <param name="stopToken"> - token used to cancel the delay.</param>
        /// <param name="handle"> - the coroutine handle to resume.</param>
        void Add(TimeSpan duration, std::stop_token stopToken, std::coroutine_handle<> handle);
    private:
        void Process();
    private:
        friend class Application;

        std::mutex _mutex;
        PendingQueue _pendings;
        std::list<StopablePending> _stopablePendings;
	};

    inline DelayDispatcher DefaultDelayDispatcher;    
}