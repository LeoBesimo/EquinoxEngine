#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

#include "eqRenderUtilities.hpp"
#include "Images/Sprite.hpp"

namespace eq
{
	class ThreadPool
	{
	private:
		std::vector<std::thread> workers;
		std::queue<std::function<void()>> tasks;
		std::mutex queueMutex;
		std::condition_variable condition;
		std::condition_variable taskFinished;
		std::atomic<int> numTasks{ 0 };
		bool stop = false;

	public:
		ThreadPool(size_t numThreads = 4);
		~ThreadPool();

		template<typename T, typename... Args>
		void queue(T task, Args... args);
		void waitForTasks();

	};
	template<typename T, typename ...Args>
	inline void ThreadPool::queue(T task, Args ...args)
	{

		{
			std::unique_lock<std::mutex> lock(queueMutex);
			numTasks++;
			//tasks.emplace([task, args...] { task(args...); });
			tasks.emplace(std::bind(task, args...));
		}
		condition.notify_one();

	}
}