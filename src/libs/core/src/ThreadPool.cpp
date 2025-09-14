#include <grabanzo/mserver/core/ThreadPool.hpp>

namespace grabanzo::mserver::core {

ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                auto task_opt = tasks.pop();
                if (!task_opt) { // La cola fue cerrada
                    break;
                }
                task_opt.value()(); // Ejecuta la tarea
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    tasks.shutdown();
    for(std::thread &worker: workers)
        worker.join();
}

} // namespace grabanzo::mserver::core
