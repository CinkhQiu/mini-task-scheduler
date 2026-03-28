#pragma once

#include "scheduler/core/SchedulerCore.h"
#include "scheduler/task/Task.h"

#include <atomic>
#include <memory>
#include <string>

namespace scheduler {

class SchedulerService {
public:
    explicit SchedulerService(std::shared_ptr<SchedulerCore> core);
    ~SchedulerService() = default;

public:
    Task::TaskId submitOnce(Task::TimePoint run_time,
                            int priority,
                            Task::Callback callback,
                            const std::string& name = "");

    Task::TaskId submitDelayed(Task::Duration delay,
                               int priority,
                               Task::Callback callback,
                               const std::string& name = "");

    Task::TaskId submitPeriodic(Task::TimePoint first_run_time,
                                Task::Duration interval,
                                int priority,
                                Task::Callback callback,
                                const std::string& name = "");

    bool cancel(Task::TaskId task_id);

private:
    Task::TaskId nextTaskId();

private:
    std::shared_ptr<SchedulerCore> core_;
    std::atomic<Task::TaskId> next_task_id_{1};
};

}  // namespace scheduler