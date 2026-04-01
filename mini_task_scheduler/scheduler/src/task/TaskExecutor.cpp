#include "scheduler/task/TaskExecutor.h"

namespace scheduler {

void TaskExecutor::execute(const TaskPtr& task) const {
    if (!task) {
        return;
    }

    if (task->isCanceled()) {
        task->setTaskStatus(TaskStatus::Canceled);
        return;
    }

    task->setTaskStatus(TaskStatus::Running);

    try {
        task->execute();

        if (task->isCanceled()) {
            task->setTaskStatus(TaskStatus::Canceled);
            return;
        }

        if (task->isPeriodic()) {
            task->setTaskStatus(TaskStatus::Pending);
        } else {
            task->setTaskStatus(TaskStatus::Completed);
        }
    } catch (const std::exception&) {
        task->setTaskStatus(TaskStatus::Failed);
    } catch (...) {
        task->setTaskStatus(TaskStatus::Failed);
    }
}

}  // namespace scheduler