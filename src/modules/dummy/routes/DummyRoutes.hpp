#pragma once

#include <grabanzo/mserver/framework/IRoute.hpp>
#include <httplib.h>

// Reemplaza 'Dummy' por el nombre de tu módulo.
namespace grabanzo::mserver::modules::dummy {

/**
 * @class DummyRoutes
 * @brief Define las rutas HTTP para el módulo de plantilla.
 *
 * Implementa la interfaz IRoute y es responsable de registrar los endpoints
 * específicos de este módulo en el servidor HTTP.
 */
class DummyRoutes : public framework::IRoute {
public:
    DummyRoutes() = default;

    /**
     * @brief Registra las rutas en el servidor HTTP.
     * @param server La instancia del servidor httplib.
     * @param prefix El prefijo de URL base para este módulo (ej. "/dummy").
     * @param context El contexto de la ruta, que contiene dependencias como el ThreadPool.
     */
    void register_routes(httplib::Server& server, const std::string& prefix, framework::RouteContext& context) override;

private:
    /**
     * @brief Manejador para la ruta de ejemplo.
     */
    void handle_dummy(const httplib::Request &req, httplib::Response &res, framework::RouteContext& context);
};

} // namespace grabanzo::mserver::modules::dummy
