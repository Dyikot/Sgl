#include "ThreadPool.h"

namespace Sgl
{
    ThreadPool& ThreadPool::Current()
    {
        //static ThreadPool threadPool(std::thread::hardware_concurrency());
        static ThreadPool threadPool(2);
        return threadPool;
    }

    int ThreadPool::GetThreadCount() const noexcept
    {
        return _workers.size();
    }

    int ThreadPool::GetPendingTaskCount() const noexcept
    {
        return _tasks.size();
    }

    void ThreadPool::QueueTask(Task task)
    {
        {
            std::lock_guard lock(_mutex);
            _tasks.emplace(std::move(task));
        }

        _semaphore.release();
    }

    ThreadPool::ThreadPool(int maxWorkers)
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

    ThreadPool::~ThreadPool()
    {
        for(auto& worker : _workers)
        {
            worker.request_stop();
        }

        _semaphore.release(_workers.size());
    }
}

