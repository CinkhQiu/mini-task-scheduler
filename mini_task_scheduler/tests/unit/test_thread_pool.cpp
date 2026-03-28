#include "scheduler/thread_pool/ThreadPool.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    scheduler::ThreadPool pool(4);

    pool.start();

    std::atomic<int> counter{0};

    for (int i = 0; i < 20; ++i) {
        pool.submit([i, &counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            ++counter;
            std::cout << "Task " << i << " executed by thread "
                      << std::this_thread::get_id() << '\n';
        });
    }

    while (counter.load() < 20) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << "All tasks finished. Counter = " << counter.load() << '\n';

    pool.stop();

    std::cout << "ThreadPool stopped successfully.\n";

    return 0;
}