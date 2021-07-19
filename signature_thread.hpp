#ifndef SIGNATURE_THREAD_HPP
#define SIGNATURE_THREAD_HPP

#include <thread>

#include "task_queue.hpp"

class signature_thread
{
    std::thread thread;
    std::atomic_bool is_active = false;

    std::function<void()> on_task_done;

    task_queue& tasks;

public:
    signature_thread(task_queue& tasks, std::function<void ()> &&on_task_done);
    signature_thread(signature_thread&& rhs);

    ~signature_thread();

    void run();

    void terminate();
};

#endif // SIGNATURE_THREAD_HPP
