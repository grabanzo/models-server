#pragma once

#include <grabanzo/mserver/framework/ServiceManager.hpp>

// Forward declarations to avoid circular dependencies
namespace grabanzo::mserver::core {
    class ThreadPool;
}

namespace grabanzo::mserver::framework {

/**
 * @struct RouteContext
 * @brief Contiene los recursos compartidos que se pasan a cada manejador de rutas.
 *
 * Esta estructura agrupa dependencias comunes para que no sea necesario
 * pasarlas como múltiples parámetros a las funciones de las rutas.
 */
struct RouteContext {
    ServiceManager& service_manager;
    core::ThreadPool& thread_pool;
};

} // namespace grabanzo::mserver::framework
