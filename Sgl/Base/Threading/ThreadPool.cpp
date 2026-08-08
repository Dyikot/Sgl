#include "ThreadPool.h"

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <semaphore>

namespace Sgl
{
    class ThreadPoolImpl
    {
    public:
        using Task = Action<>;
    public:
        ThreadPoolImpl(size_t maxWorkers)
        {
            _workers.reserve(maxWorkers);

            for(int i = 0; i < maxWorkers; i++)
            {
                _workers.emplace_back([this](std::stop_token stopToken)
                {
                    while(!stopToken.stop_requested())
                    {
                        Task task;

                        _semaphore.acquire();

                        if(stopToken.stop_requested())
                        {
                            break;
                        }

                        {
                            std::lock_guard lock(_mutex);

                            if(_tasks.empty())
                            {
                                _semaphore.release();
                                continue;
                            }

                            task = std::move(_tasks.front());
                            _tasks.pop();
                        }

                        if(task.HasTarget())
                        {
                            task();
                        }
                    }
                });
            }
        }

        ThreadPoolImpl(const ThreadPool&) = delete;
        ThreadPoolImpl(ThreadPool&&) = delete;

        ~ThreadPoolImpl()
        {
            for(auto& worker : _workers)
            {
                worker.request_stop();
            }

            _semaphore.release(_workers.size());
        }

        static inline constinit size_t MaxThreads = 4;

        static ThreadPoolImpl& Instance()
        {
            static ThreadPoolImpl threadPool(MaxThreads);
            return threadPool;
        }

        void Queue(Task task)
        {
            {
                std::lock_guard lock(_mutex);
                _tasks.emplace(std::move(task));
            }

            _semaphore.release();
        }

        friend class ThreadPool;
    private:
        std::queue<Task> _tasks;
        std::vector<std::jthread> _workers;
        std::mutex _mutex;
        std::counting_semaphore<> _semaphore { 0 };
    };

    size_t ThreadPool::GetThreadCount() noexcept
    {
        return ThreadPoolImpl::Instance()._workers.size();
    }

    size_t ThreadPool::GetPendingTaskCount() noexcept
    {
        auto& impl = ThreadPoolImpl::Instance();
        std::lock_guard lock(impl._mutex);
        return impl._tasks.size();
    }

    void ThreadPool::QueueTask(Task task)
    {
        ThreadPoolImpl::Instance().Queue(std::move(task));
    }

    void ThreadPool::SetThreadCount(size_t threads) noexcept
    {
        ThreadPoolImpl::Instance().MaxThreads = threads;
    }
}

