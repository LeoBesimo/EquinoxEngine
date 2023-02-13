#include "eqThreadPool.hpp"

eq::ThreadPool::ThreadPool(size_t numThreads)
{
    for (size_t i = 0; i < numThreads; i++) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty())
                        return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
                numTasks--;
                taskFinished.notify_one();
            }
            });
    }
}

eq::ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (auto& worker : workers)
        worker.join();
}

void eq::ThreadPool::waitForTasks()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    taskFinished.wait(lock, [this] { return numTasks == 0; });
}
