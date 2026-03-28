#pragma once

#include "scheduler/core/SchedulerCore.h"
#include "scheduler/task/Task.h"

#include <atomic>
#include <memory>
#include <string>

namespace scheduler {
/**
 * @brief 调度服务层
 *
 * 该类对外提供统一的任务提交与取消接口，
 * 支持一次性任务、延时任务和周期任务。
 *
 * 同时，该类内部维护了一个任务索引表，
 * 用于根据 task_id 查找并取消对应任务。
 *
 * 当前版本说明：
 * 1. 已执行完成的任务，暂时不会自动从内部任务表中移除。
 * 2. 已取消的周期任务，暂时也不会自动从内部任务表中清理。
 * 3. 异常处理策略尚未最终确定，当前版本以基础功能跑通为主。
 *
 * TODO:
 * 1. 为已完成任务增加自动清理机制，避免任务索引表持续增长。
 * 2. 为已取消任务增加清理策略，进一步完善任务生命周期管理。
 * 3. 增加任务查询能力，例如按 task_id 查询任务状态。
 * 4. 明确异常处理策略，区分参数非法、调度失败、重复取消等场景。
 * 5. 补充线程安全边界验证，完善高并发场景下的稳定性测试。
 * 6. 后续可考虑支持 future / 返回值，以增强任务执行结果获取能力。
 */
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
    
    mutable std::mutex mutex_;
    std::unordered_map<Task::TaskId, TaskPtr> tasks_;
};

}  // namespace scheduler