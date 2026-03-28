#pragma once

#include "scheduler/task/Task.h"

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <vector>
#include <functional>

namespace scheduler {

class TaskQueue {
public:
    using TimePoint = Task::TimePoint;
    using QueueType = std::priority_queue<TaskPtr, std::vector<TaskPtr>, TaskCompare>;

public:
    TaskQueue() = default;
    ~TaskQueue() = default;

    void push(const TaskPtr& task);

    TaskPtr pop();

    TaskPtr top() const;

    bool empty() const;

    std::size_t size() const;

    void wait(const std::function<bool()>& stop_predicate);

    void waitUntil(TimePoint time_point, const std::function<bool()>& stop_predicate);

    void notifyOne();

    void notifyAll();

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    QueueType queue_;
};

}  // namespace scheduler