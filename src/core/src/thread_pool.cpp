#include <grabanzo/mserver/thread_pool.hpp>
#include <grabanzo/mserver/thread_pool_config.hpp>

namespace grabanzo::mserver {

ThreadPool::ThreadPool(const ThreadPoolConfig& config)
  : config_{ config }
  , stop_(false)
{
    for (size_t i = 0; i < config_.size; ++i) {
        workers_.emplace_back([this] {
            while (true) {
                auto task_opt = tasks_.pop();
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
    tasks_.shutdown();
    for (std::thread& worker : workers_)
        worker.join();
}

} // namespace grabanzo::mserver::core
