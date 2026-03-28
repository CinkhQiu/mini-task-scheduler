#include "scheduler/task/TaskQueue.h"

namespace scheduler {

void TaskQueue::push(const TaskPtr& task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(task);
    }
    cv_.notify_one();
}

TaskPtr TaskQueue::pop() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (queue_.empty()) {
        return nullptr;
    }

    TaskPtr task = queue_.top();
    queue_.pop();
    return task;
}

TaskPtr TaskQueue::top() const {
    std::lock_guard<std::mutex> lock(mutex_);

    if (queue_.empty()) {
        return nullptr;
    }

    return queue_.top();
}

bool TaskQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

std::size_t TaskQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

void TaskQueue::wait(const std::function<bool()>& stop_predicate) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this, &stop_predicate]() {
        return !queue_.empty() || stop_predicate();
    });
}

void TaskQueue::waitUntil(TimePoint time_point, const std::function<bool()>& stop_predicate) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait_until(lock, time_point, [this, &stop_predicate]() {
        return !queue_.empty() || stop_predicate();
    });
}

void TaskQueue::notifyOne() {
    cv_.notify_one();
}

void TaskQueue::notifyAll() {
    cv_.notify_all();
}

}  // namespace scheduler