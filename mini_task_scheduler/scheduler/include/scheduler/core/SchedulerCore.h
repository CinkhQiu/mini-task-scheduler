#pragma once

#include "scheduler/task/Task.h"
#include "scheduler/task/TaskQueue.h"
#include "scheduler/thread_pool/ThreadPool.h"

#include <atomic>
#include <memory>
#include <thread>

namespace scheduler {

class SchedulerCore {
public:
    using Clock = Task::Clock;
    using TimePoint = Task::TimePoint;
    using Duration = Task::Duration;

public:
    explicit SchedulerCore(std::shared_ptr<ThreadPool> thread_pool);
    ~SchedulerCore();

public:
    // Start the scheduler
    void start();
    // Stop the scheduler and wait for the worker thread to finish
    void stop();    

    // Schedule a new task to scheduler
    void submit(const TaskPtr& task);

    bool isRunning() const noexcept;

private:
    void run();

    // Check if the task should be executed now
    bool shouldRunNow(const TaskPtr& task, TimePoint now) const noexcept;

    void dispatchTask(const TaskPtr& task);

    void reschedulePeriodicTask(const TaskPtr& task);

private:
    std::atomic<bool> running_{false};
    TaskQueue task_queue_;
    std::shared_ptr<ThreadPool> thread_pool_;
    std::thread worker_;
};

}  // namespace scheduler