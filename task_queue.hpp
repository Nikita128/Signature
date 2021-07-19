#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "common/types.hpp"

class task_queue
{
    std::queue<signature_task> queue;

    std::mutex m;
    std::condition_variable cond_var;

    std::atomic_bool is_terminated = false;

public:
    void push(signature_task&& task);

    signature_task pop();

    void terminate();
};

#endif // TASK_QUEUE_HPP
