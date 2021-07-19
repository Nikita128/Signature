#include "thread_pool.hpp"

thread_pool::thread_pool()
{
    auto size = std::thread::hardware_concurrency();
    //auto size = 1;
    if (!size)
    {
        size = 1;
    }

    pool.reserve(size);

    for (size_t i = 0; i < size; ++i)
    {
        pool.emplace_back(
            tasks,
            [&]()
            {
                if (tasks_in_progress)
                {
                    --tasks_in_progress;
                }
            });
        pool.back().run();
    }
}

thread_pool::~thread_pool()
{
    for (auto& thread : pool)
    {
        thread.terminate();
    }

    tasks.terminate();
}

size_t thread_pool::size()
{
    return pool.size();
}

void thread_pool::wait()
{
    while (tasks_in_progress)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void thread_pool::execute_task(signature_task &&task)
{
    ++tasks_in_progress;
    tasks.push(std::move(task));
}
