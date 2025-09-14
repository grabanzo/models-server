#pragma once

#include <string>

// Forward declarations to avoid including full headers here.
namespace httplib {
    class Server;
}

namespace grabanzo::mserver::framework {

class RouteContext; // Forward declaration for our own type

/**
 * @class IRoute
 * @brief Interfaz base para todas las clases que definen rutas.
 *
 * Esta clase abstracta define un contrato común para registrar colecciones
 * de rutas relacionadas en una instancia de `httplib::Server`.
 */
class IRoute {
public:
    virtual ~IRoute() = default;

    /**
     * @brief Registra todas las rutas definidas por la clase en el servidor.
     * @param server La instancia de `httplib::Server` donde se registrarán las rutas.
     * @param prefix El prefijo de ruta base proporcionado por el módulo padre.
     * @param context Un objeto que contiene recursos compartidos (ThreadPool, ServiceManager, etc).
     */
    virtual void register_routes(httplib::Server& server, const std::string& prefix, RouteContext& context) = 0;
};

} // namespace grabanzo::mserver::framework
