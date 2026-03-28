#include "scheduler/core/SchedulerCore.h"
#include "scheduler/task/Task.h"
#include "scheduler/thread_pool/ThreadPool.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

int main() {
    using namespace scheduler;

    auto thread_pool = std::make_shared<ThreadPool>(4);
    thread_pool->start();

    SchedulerCore core(thread_pool);
    core.start();

    std::atomic<int> delayed_counter{0};
    std::atomic<int> periodic_counter{0};
    std::atomic<int> canceled_counter{0};

    auto now = Task::Clock::now();

    auto delayed_task = std::make_shared<Task>(
        1,
        TaskType::Delayed,
        now + std::chrono::milliseconds(200),
        std::chrono::milliseconds(0),
        1,
        [&delayed_counter]() {
            ++delayed_counter;
            std::cout << "[Delayed Task] executed\n";
        },
        "delayed_task"
    );

    auto periodic_task = std::make_shared<Task>(
        2,
        TaskType::Periodic,
        now + std::chrono::milliseconds(100),
        std::chrono::milliseconds(100),
        1,
        [&periodic_counter]() {
            ++periodic_counter;
            std::cout << "[Periodic Task] executed\n";
        },
        "periodic_task"
    );

    auto canceled_task = std::make_shared<Task>(
        3,
        TaskType::Delayed,
        now + std::chrono::milliseconds(150),
        std::chrono::milliseconds(0),
        1,
        [&canceled_counter]() {
            ++canceled_counter;
            std::cout << "[Canceled Task] executed\n";
        },
        "canceled_task"
    );

    canceled_task->cancel();

    core.submit(delayed_task);
    core.submit(periodic_task);
    core.submit(canceled_task);

    std::this_thread::sleep_for(std::chrono::milliseconds(550));

    periodic_task->cancel();

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    core.stop();
    thread_pool->stop();

    std::cout << "\n===== Test Result =====\n";
    std::cout << "Delayed counter  : " << delayed_counter.load() << '\n';
    std::cout << "Periodic counter : " << periodic_counter.load() << '\n';
    std::cout << "Canceled counter : " << canceled_counter.load() << '\n';

    bool delayed_ok = (delayed_counter.load() == 1);
    bool periodic_ok = (periodic_counter.load() >= 3);
    bool canceled_ok = (canceled_counter.load() == 0);

    if (delayed_ok && periodic_ok && canceled_ok) {
        std::cout << "SchedulerCore test passed.\n";
        return 0;
    }

    std::cout << "SchedulerCore test failed.\n";
    return 1;
}