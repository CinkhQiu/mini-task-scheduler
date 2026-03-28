#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace scheduler {

enum class TaskType {
    Once,
    Delayed,
    Periodic
};

class Task {
public:
    using TaskId = std::uint64_t;
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::milliseconds;
    using Callback = std::function<void()>;

public:
    Task(TaskId id,
         TaskType type,
         TimePoint next_run_time,
         Duration interval,
         int priority,
         Callback callback,
         std::string name = "")
        : id_(id),
          type_(type),
          next_run_time_(next_run_time),
          interval_(interval),
          priority_(priority),
          callback_(std::move(callback)),
          name_(std::move(name)),
          canceled_(false) {}

    ~Task() = default;

public:
    TaskId id() const noexcept {
        return id_;
    }

    TaskType type() const noexcept {
        return type_;
    }

    const std::string& name() const noexcept {
        return name_;
    }

    TimePoint nextRunTime() const noexcept {
        return next_run_time_;
    }

    Duration interval() const noexcept {
        return interval_;
    }

    int priority() const noexcept {
        return priority_;
    }

    bool isCanceled() const noexcept {
        return canceled_.load(std::memory_order_relaxed);
    }

    bool isPeriodic() const noexcept {
        return type_ == TaskType::Periodic;
    }

public:
    void cancel() noexcept {
        canceled_.store(true, std::memory_order_relaxed);
    }

    void execute() const {
        if (isCanceled()) {
            return;
        }

        if (callback_) {
            callback_();
        }
    }

    void updateNextRunTime() noexcept {
        if (type_ == TaskType::Periodic) {
            next_run_time_ += interval_;
        }
    }

private:
    TaskId id_;
    TaskType type_;
    TimePoint next_run_time_;
    Duration interval_;
    int priority_;
    Callback callback_;
    std::string name_;
    std::atomic<bool> canceled_;
};

using TaskPtr = std::shared_ptr<Task>;

struct TaskCompare {
    bool operator()(const TaskPtr& lhs, const TaskPtr& rhs) const {
        if (lhs->nextRunTime() != rhs->nextRunTime()) {
            return lhs->nextRunTime() > rhs->nextRunTime();
        }

        if (lhs->priority() != rhs->priority()) {
            return lhs->priority() < rhs->priority();
        }

        return lhs->id() > rhs->id();
    }
};

}  // namespace scheduler