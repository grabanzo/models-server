#include <grabanzo/mserver/modules/health/health_module.hpp>
#include <grabanzo/mserver/modules/health/health_routes.hpp>

namespace grabanzo::mserver::modules {

std::vector<std::unique_ptr<grabanzo::mserver::Route>> HealthModule::getRouteHandlers() {
    auto routes = std::vector<std::unique_ptr<grabanzo::mserver::Route>>();
    routes.push_back(std::make_unique<HealthRoutes>());
    return routes;
}

std::string HealthModule::getPrefix() const {
    return "/health";
}

} // namespace grabanzo::mserver::modules::health
