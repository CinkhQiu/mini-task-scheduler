#pragma once

#include "scheduler/core/SchedulerCore.h"
#include "scheduler/service/SchedulerService.h"
#include "scheduler/thread_pool/ThreadPool.h"

#include <cstddef>
#include <memory>

namespace scheduler {

class Scheduler {
public:
    explicit Scheduler(std::size_t thread_count);
    ~Scheduler();

public:
    void start();
    void stop();

    bool isRunning() const noexcept;

    SchedulerService& service() noexcept;
    const SchedulerService& service() const noexcept;

private:
    std::shared_ptr<ThreadPool> thread_pool_;
    std::shared_ptr<SchedulerCore> core_;
    std::shared_ptr<SchedulerService> service_;
};

}  // namespace scheduler