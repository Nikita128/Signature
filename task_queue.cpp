#include "task_queue.hpp"

void task_queue::push(signature_task &&task)
{
    {
        std::unique_lock lock(m);
        queue.push(std::move(task));
    }
    cond_var.notify_one();
}

signature_task task_queue::pop()
{
    std::unique_lock lock(m);

    while (queue.empty())
    {
        if (is_terminated)
        {
            return {};
        }

        cond_var.wait(lock);
    }

    auto task = std::move(queue.front());
    queue.pop();

    return task;
}

void task_queue::terminate()
{
    is_terminated = true;

    cond_var.notify_all();
}
