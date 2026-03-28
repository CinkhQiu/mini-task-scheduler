#include "scheduler/service/SchedulerService.h"

#include <chrono>
#include <utility>

namespace scheduler {

SchedulerService::SchedulerService(std::shared_ptr<SchedulerCore> core)
    : core_(std::move(core)) {
}

Task::TaskId SchedulerService::submitOnce(Task::TimePoint run_time,
                                          int priority,
                                          Task::Callback callback,
                                          const std::string& name) {
    if (!core_ || !callback) {
        return 0;
    }

    Task::TaskId task_id = nextTaskId();

    TaskPtr task = std::make_shared<Task>(
        task_id,
        TaskType::Once,
        run_time,
        Task::Duration(0),
        priority,
        std::move(callback),
        name
    );

    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_[task_id] = task;
    }

    core_->submit(task);
    return task_id;
}

Task::TaskId SchedulerService::submitDelayed(Task::Duration delay,
                                             int priority,
                                             Task::Callback callback,
                                             const std::string& name) {
    if (!core_ || !callback) {
        return 0;
    }

    Task::TimePoint run_time = Task::Clock::now() + delay;
    Task::TaskId task_id = nextTaskId();

    TaskPtr task = std::make_shared<Task>(
        task_id,
        TaskType::Delayed,
        run_time,
        Task::Duration(0),
        priority,
        std::move(callback),
        name
    );

    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_[task_id] = task;
    }

    core_->submit(task);
    return task_id;
}

Task::TaskId SchedulerService::submitPeriodic(Task::TimePoint first_run_time,
                                              Task::Duration interval,
                                              int priority,
                                              Task::Callback callback,
                                              const std::string& name) {
    if (!core_ || !callback || interval.count() <= 0) {
        return 0;
    }

    Task::TaskId task_id = nextTaskId();

    TaskPtr task = std::make_shared<Task>(
        task_id,
        TaskType::Periodic,
        first_run_time,
        interval,
        priority,
        std::move(callback),
        name
    );

    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_[task_id] = task;
    }

    core_->submit(task);
    return task_id;
}

bool SchedulerService::cancel(Task::TaskId task_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = tasks_.find(task_id);
    if (it == tasks_.end() || !it->second) {
        return false;
    }

    it->second->cancel();
    return true;
}

Task::TaskId SchedulerService::nextTaskId() {
    return next_task_id_.fetch_add(1, std::memory_order_relaxed);
}

}  // namespace scheduler