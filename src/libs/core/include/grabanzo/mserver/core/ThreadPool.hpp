#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <future>
#include <stdexcept>

#include <grabanzo/mserver/core/ThreadSafeQueue.hpp>

namespace grabanzo::mserver::core {

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
    ThreadPool(size_t num_threads);

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
    std::vector<std::thread> workers;
    ThreadSafeQueue<std::function<void()>> tasks;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
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
    tasks.push([task](){ (*task)(); });
    return res;
}

} // namespace grabanzo::mserver::core