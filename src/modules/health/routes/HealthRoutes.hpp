#pragma once

// Forward declarations
namespace httplib {
    class Server;
    struct Request;
    struct Response;
}

// Full include for base class
#include <grabanzo/mserver/framework/IRoute.hpp>

namespace grabanzo::mserver::modules::health {

// Define las rutas relacionadas con la salud y el estado del servicio.
class HealthRoutes : public framework::IRoute {
public:
    HealthRoutes() = default;
    void register_routes(httplib::Server& server, const std::string& prefix, framework::RouteContext& context) override;

private:
    void handle_health(const httplib::Request &req, httplib::Response &res, framework::RouteContext& context);
};

} // namespace grabanzo::mserver::modules::health
