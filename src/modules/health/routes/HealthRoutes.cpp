#include <httplib.h>

#include <grabanzo/mserver/core/Logger.hpp>
#include <grabanzo/mserver/framework/RouteContext.hpp>

#include "HealthRoutes.hpp"

namespace grabanzo::mserver::modules::health {

using namespace grabanzo::mserver::framework;

void HealthRoutes::register_routes(httplib::Server& server, const std::string& prefix, RouteContext& context) {
    server.Get(prefix + "/", [this, &context](const httplib::Request &req, httplib::Response &res) {
        handle_health(req, res, context);
    });
}

void HealthRoutes::handle_health(const httplib::Request &req, httplib::Response &res, RouteContext& context) {
    res.set_content("{\"status\": \"ok\"}", "application/json");
}

} // namespace grabanzo::mserver::modules::health
