#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "signature_thread.hpp"
#include "task_queue.hpp"

class thread_pool
{
    task_queue tasks;

    std::vector<signature_thread> pool;
    std::atomic_int tasks_in_progress = 0;

public:
    thread_pool();
    ~thread_pool();

    size_t size();
    void wait();

    void execute_task(signature_task&& task);
};

#endif // THREAD_POOL_HPP
