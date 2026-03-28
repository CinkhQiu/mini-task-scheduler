#include "scheduler/core/SchedulerCore.h"

#include <chrono>
#include <utility>

namespace scheduler {

SchedulerCore::SchedulerCore(std::shared_ptr<ThreadPool> thread_pool)
    : thread_pool_(std::move(thread_pool)) {
}

SchedulerCore::~SchedulerCore() {
    stop();
}

void SchedulerCore::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) {
        return;
    }

    worker_ = std::thread(&SchedulerCore::run, this);
}

void SchedulerCore::stop() {
    bool expected = true;
    if (!running_.compare_exchange_strong(expected, false)) {
        return;
    }

    task_queue_.notifyAll();

    if (worker_.joinable()) {
        worker_.join();
    }
}

void SchedulerCore::submit(const TaskPtr& task) {
    if (!task) {
        return;
    }

    task_queue_.push(task);
}

bool SchedulerCore::isRunning() const noexcept {
    return running_.load(std::memory_order_relaxed);
}

void SchedulerCore::run() {
    while (running_.load(std::memory_order_relaxed)) {
        TaskPtr task = task_queue_.top();

        if (!task) {
            task_queue_.wait();
            continue;
        }

        TimePoint now = Clock::now();

        if (!shouldRunNow(task, now)) {
            task_queue_.waitUntil(task->nextRunTime());
            continue;
        }

        task = task_queue_.pop();
        if (!task) {
            continue;
        }

        if (task->isCanceled()) {
            continue;
        }

        dispatchTask(task);

        if (task->isPeriodic() && !task->isCanceled()) {
            reschedulePeriodicTask(task);
        }
    }
}

bool SchedulerCore::shouldRunNow(const TaskPtr& task, TimePoint now) const noexcept {
    if (!task) {
        return false;
    }

    return task->nextRunTime() <= now;
}

void SchedulerCore::dispatchTask(const TaskPtr& task) {
    if (!task || !thread_pool_) {
        return;
    }

    thread_pool_->submit([task]() {
        task->execute();
    });
}

void SchedulerCore::reschedulePeriodicTask(const TaskPtr& task) {
    if (!task) {
        return;
    }

    task->updateNextRunTime();
    task_queue_.push(task);
}

}  // namespace scheduler