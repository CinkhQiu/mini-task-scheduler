#pragma once

#include "scheduler/task/Task.h"

/**
 * @brief 任务执行器
 *
 * 该类负责执行单个任务对象，
 * 当前版本仅包含最基础的执行逻辑：
 * 1. 判空
 * 2. 判断任务是否已取消
 * 3. 调用任务回调执行
 *
 * TODO:
 * 1. 增加 try-catch 机制，避免任务执行异常导致线程异常退出。
 * 2. 增加执行日志，记录任务开始、结束、取消、异常等关键状态。
 * 3. 增加任务执行耗时统计，便于后续性能分析与监控。
 * 4. 增加任务执行结果状态封装，为后续任务查询能力提供基础。
 * 5. 后续可考虑接入统一日志模块（AsyncLogger），实现异步记录。
 */
namespace scheduler {

class TaskExecutor {
public:
    TaskExecutor() = default;
    ~TaskExecutor() = default;

public:
    void execute(const TaskPtr& task) const;
};

}  // namespace scheduler