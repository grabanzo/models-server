#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <future>

#include <grabanzo/mserver/thread_queue.hpp>
#include <grabanzo/mserver/thread_pool_config.hpp>


namespace grabanzo::mserver {

/**
 * @class ThreadPool
 * @brief Gestiona un conjunto de hilos trabajadores para ejecutar tareas de forma concurrente.
 *
 * Implementa el patrón productor-consumidor. Las tareas se encolan y los hilos
 * disponibles las van procesando.
 */
class ThreadPool {
public:
    /**
     * @brief Constructor que inicia el número especificado de hilos trabajadores.
     * @param num_threads El número de hilos a crear.
     */
    ThreadPool(const ThreadPoolConfig& config);

    ~ThreadPool();
    
    /**
     * @brief Encola una nueva tarea para ser ejecutada por el pool de hilos.
     * @tparam F El tipo de la función o callable a ejecutar.
     * @tparam Args Los tipos de los argumentos de la función.
     * @param f La función o callable.
     * @param args Los argumentos que se pasarán a la función.
     * @return Un `std::future` que contendrá el resultado de la ejecución de la tarea.
     */
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>>;

private:
    std::vector<std::thread> workers_;
    ThreadQueue<std::function<void()>> tasks_;
    ThreadPoolConfig config_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

// --- Implementación del template ---
// (Es necesario que esté en el header)

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>> {
    using return_type = typename std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
        
    std::future<return_type> res = task->get_future();
    tasks_.push([task](){ (*task)(); });
    return res;
}

} // namespace grabanzo::mserver::core