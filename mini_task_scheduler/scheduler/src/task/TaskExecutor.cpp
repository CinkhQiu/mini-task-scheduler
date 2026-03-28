#include "scheduler/task/TaskExecutor.h"

namespace scheduler {

void TaskExecutor::execute(const TaskPtr& task) const {
    if (!task) {
        return;
    }

    if (task->isCanceled()) {
        return;
    }

    task->execute();
}

}  // namespace scheduler