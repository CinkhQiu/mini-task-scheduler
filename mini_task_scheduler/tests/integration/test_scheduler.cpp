#include "scheduler/core/Scheduler.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    using namespace scheduler;

    Scheduler scheduler(4);
    scheduler.start();

    std::atomic<int> once_counter{0};
    std::atomic<int> delayed_counter{0};
    std::atomic<int> periodic_counter{0};
    std::atomic<int> canceled_counter{0};

    auto now = Task::Clock::now();

    auto once_task_id = scheduler.service().submitOnce(
        now + std::chrono::milliseconds(100),
        1,
        [&once_counter]() {
            ++once_counter;
            std::cout << "[Once Task] executed\n";
        },
        "once_task"
    );

    auto delayed_task_id = scheduler.service().submitDelayed(
        std::chrono::milliseconds(200),
        1,
        [&delayed_counter]() {
            ++delayed_counter;
            std::cout << "[Delayed Task] executed\n";
        },
        "delayed_task"
    );

    auto periodic_task_id = scheduler.service().submitPeriodic(
        now + std::chrono::milliseconds(50),
        std::chrono::milliseconds(100),
        1,
        [&periodic_counter]() {
            ++periodic_counter;
            std::cout << "[Periodic Task] executed\n";
        },
        "periodic_task"
    );

    auto canceled_task_id = scheduler.service().submitDelayed(
        std::chrono::milliseconds(150),
        1,
        [&canceled_counter]() {
            ++canceled_counter;
            std::cout << "[Canceled Task] executed\n";
        },
        "canceled_task"
    );

    bool cancel_ok = scheduler.service().cancel(canceled_task_id);
    std::cout << "Cancel result: " << (cancel_ok ? "true" : "false") << '\n';

    std::this_thread::sleep_for(std::chrono::milliseconds(550));

    bool periodic_cancel_ok = scheduler.service().cancel(periodic_task_id);
    std::cout << "Periodic cancel result: " << (periodic_cancel_ok ? "true" : "false") << '\n';

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    scheduler.stop();

    std::cout << "\n===== Test Result =====\n";
    std::cout << "once_task_id      : " << once_task_id << '\n';
    std::cout << "delayed_task_id   : " << delayed_task_id << '\n';
    std::cout << "periodic_task_id  : " << periodic_task_id << '\n';
    std::cout << "canceled_task_id  : " << canceled_task_id << '\n';

    std::cout << "Once counter      : " << once_counter.load() << '\n';
    std::cout << "Delayed counter   : " << delayed_counter.load() << '\n';
    std::cout << "Periodic counter  : " << periodic_counter.load() << '\n';
    std::cout << "Canceled counter  : " << canceled_counter.load() << '\n';

    bool once_ok = (once_counter.load() == 1);
    bool delayed_ok = (delayed_counter.load() == 1);
    bool periodic_ok = (periodic_counter.load() >= 3);
    bool canceled_ok = (canceled_counter.load() == 0);

    if (once_ok && delayed_ok && periodic_ok && canceled_ok) {
        std::cout << "Scheduler integration test passed.\n";
        return 0;
    }

    std::cout << "Scheduler integration test failed.\n";
    return 1;
}