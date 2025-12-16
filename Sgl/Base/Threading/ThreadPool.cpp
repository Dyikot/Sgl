#include "ThreadPool.h"

#include <mutex>
#include <queue>
#include <thread>
#include <semaphore>

namespace Sgl
{
    class ThreadPoolImpl
    {
    private:
        using Task = Action<>;

        std::queue<Task> _tasks;
        std::vector<std::jthread> _workers;
        std::mutex _mutex;
        std::counting_semaphore<> _semaphore { 0 };
    public:
        ThreadPoolImpl(int maxWorkers)
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
                            _semaphore.release();
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

        void Queue(Task task)
        {
            {
                std::lock_guard lock(_mutex);
                _tasks.emplace(std::move(task));
            }

            _semaphore.release();
        }

        friend class ThreadPool;
    };

    static size_t _maxThreads = 4;

    static ThreadPoolImpl& GetThreadPoolImpl()
    {
        static ThreadPoolImpl threadPoolImpl(_maxThreads);
        return threadPoolImpl;
    }    

    size_t ThreadPool::GetThreadCount() noexcept
    {
        return GetThreadPoolImpl()._workers.size();
    }

    size_t ThreadPool::GetPendingTaskCount() noexcept
    {
        return GetThreadPoolImpl()._tasks.size();
    }

    void ThreadPool::QueueTask(Task task)
    {
        GetThreadPoolImpl().Queue(std::move(task));
    }

    void ThreadPool::SetThreadCount(size_t threads) noexcept
    {
        _maxThreads = threads;
    }
}

