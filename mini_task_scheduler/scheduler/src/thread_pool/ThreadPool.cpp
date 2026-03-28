#include "scheduler/thread_pool/ThreadPool.h"

using namespace scheduler;

ThreadPool::ThreadPool(std::size_t thread_count)
    : thread_count_(thread_count == 0 ? 1 : thread_count) {}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) {
        return;  // Already running
    }

    workers_.reserve(thread_count_);

    for (std::size_t i = 0; i < thread_count_; ++i) {
        workers_.emplace_back(&ThreadPool::workerLoop, this);
    }
}

void ThreadPool::stop() {
    bool expected = true;
    if (!running_.compare_exchange_strong(expected, false)) {
        return;  // Already stopped
    }

    cv_.notify_all();

    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();
}

void ThreadPool::submit(Task task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
    }
    cv_.notify_one();
}

bool ThreadPool::isRunning() const noexcept {
    return running_.load();
}

std::size_t ThreadPool::threadCount() const noexcept {
    return thread_count_;
}

void ThreadPool::workerLoop() {
    while (true) {
        Task task;

        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this]() {
                return !running_.load() || !tasks_.empty();
            });
            if (!running_.load() && tasks_.empty()) {
                break;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
        }

        if (task) {
            task();
        }
    }
}