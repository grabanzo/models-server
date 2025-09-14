#pragma once

#include <functional>

// Forward declarations to avoid circular dependencies
namespace grabanzo::mserver::core {
    class ThreadPool;
}

namespace grabanzo::mserver::framework {

class ServiceManager; // Belongs to the same namespace

/**
 * @struct RouteContext
 * @brief Contiene los recursos compartidos que se pasan a cada manejador de rutas.
 *
 * Esta estructura agrupa dependencias comunes para que no sea necesario
 * pasarlas como múltiples parámetros a las funciones de las rutas.
 */
struct RouteContext {
    core::ThreadPool& thread_pool;
    ServiceManager& service_manager;
    std::function<void()> pre_request_handler;
};

} // namespace grabanzo::mserver::framework
