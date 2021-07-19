#include "signature_thread.hpp"
#include <iostream>
signature_thread::signature_thread(task_queue &tasks, std::function<void ()> &&on_task_done) :
    on_task_done(std::move(on_task_done)),
    tasks(tasks)
{}

signature_thread::signature_thread(signature_thread &&rhs) :
    on_task_done(std::move(rhs.on_task_done)),
    tasks(rhs.tasks)
{
    if (rhs.is_active)
    {
        throw std::runtime_error("Can't move active thread");
    }
}

signature_thread::~signature_thread()
{
    if (thread.joinable())
    {
        thread.join();
    }
}

void signature_thread::terminate()
{
    is_active = false;
}

void signature_thread::run()
{
    if (is_active)
    {
        return;
    }

    is_active = true;

    std::thread thr (
    [&]()
    {
        while (is_active)
        {
            auto task = tasks.pop();

            if (task)
            {
                task();
                on_task_done();
            }
        }
    });

    thread = std::move(thr);
}
