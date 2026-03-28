#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace scheduler {

class ThreadPool {
public:
    using Task = std::function<void()>;

public:
    explicit ThreadPool(std::size_t thread_count);
    ~ThreadPool();

public:
    void start();
    void stop();

    void submit(Task task);

    bool isRunning() const noexcept;
    std::size_t threadCount() const noexcept;

private:
    void workerLoop();

private:
    std::size_t thread_count_{0};
    std::vector<std::thread> workers_;

    std::queue<Task> tasks_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;

    std::atomic<bool> running_{false};
};

}  // namespace scheduler