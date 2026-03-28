#include "scheduler/core/Scheduler.h"

namespace scheduler {

Scheduler::Scheduler(std::size_t thread_count) {
    thread_pool_ = std::make_shared<ThreadPool>(thread_count);
    core_ = std::make_shared<SchedulerCore>(thread_pool_);
    service_ = std::make_shared<SchedulerService>(core_);
}

Scheduler::~Scheduler() {
    stop();
}

void Scheduler::start() {
    if (thread_pool_) {
        thread_pool_->start();
    }

    if (core_) {
        core_->start();
    }
}

void Scheduler::stop() {
    if (core_) {
        core_->stop();
    }

    if (thread_pool_) {
        thread_pool_->stop();
    }
}

bool Scheduler::isRunning() const noexcept {
    return core_ ? core_->isRunning() : false;
}

SchedulerService& Scheduler::service() noexcept {
    return *service_;
}

const SchedulerService& Scheduler::service() const noexcept {
    return *service_;
}

}  // namespace scheduler