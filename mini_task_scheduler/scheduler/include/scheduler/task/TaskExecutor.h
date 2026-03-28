#pragma once

#include "scheduler/task/Task.h"

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <vector>

namespace  scheduler {
class TaskExecutor {
public:    
    TaskExecutor() = default;
    ~TaskExecutor() = default;

public:
    void push(const TaskPtr& task);

    TaskPtr pop();

    TaskPtr top() const;

    bool empty() const;

    std::size_t size() const;

    void wait();

    void waitUntil(Task::TimePoint time_point);

    void notifyAll();

private:
    using QueueType = std::priority_queue<TaskPtr, std::vector<TaskPtr>, TaskCompare>;

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    QueueType queue_;
};
}