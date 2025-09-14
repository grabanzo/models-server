#pragma once

#include <memory>
#include <string>
#include <vector>

// Forward declarations
namespace httplib {
    class Server;
}
namespace grabanzo::mserver::core {
    class ThreadPool;
}

namespace grabanzo::mserver::framework {

// Forward declarations for types within the same library
class IRoute;
class IService;
class ServiceManager;

/**
 * @class IModule
 * @brief Interfaz que define un módulo autocontenido de la aplicación.
 *
 * Un módulo agrupa un conjunto de funcionalidades relacionadas, incluyendo:
 * - Servicios que puede registrar.
 * - Rutas HTTP que puede manejar.
 * - Un prefijo de URL bajo el cual operan sus rutas.
 */
class IModule {
public:
    virtual ~IModule() = default;

    /**
     * @brief Registra los servicios que el módulo proporciona.
     * @param manager El ServiceManager central donde se registran los servicios.
     */
    virtual void register_services(ServiceManager& manager) = 0;

    /**
     * @brief Obtiene los manejadores de rutas HTTP que este módulo implementa.
     * @return Un vector de punteros únicos a objetos que implementan IRoute.
     */
    virtual std::vector<std::unique_ptr<IRoute>> get_route_handlers() = 0;

    /**
     * @brief Obtiene el prefijo de la URL para todas las rutas de este módulo.
     * @return Un string que representa el prefijo (ej. "/health", "/face").
     *         Si se devuelve una cadena vacía, las rutas se registrarán en la raíz.
     */
    virtual std::string get_prefix() const { return ""; }
};

} // namespace grabanzo::mserver::framework
