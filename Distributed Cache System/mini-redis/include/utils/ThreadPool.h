#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <stdexcept>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    
    void workerThread();
    
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();
    
    template<typename F>
    void enqueue(F&& task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("ThreadPool is stopped");
            }
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }
    
    size_t size() const;
};

#endif // THREAD_POOL_H
