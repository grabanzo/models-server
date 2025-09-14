#include <grabanzo/mserver/framework/ModuleRegistry.hpp>
#include "HealthModule.hpp"
#include "routes/HealthRoutes.hpp"

namespace grabanzo::mserver::modules::health {

void HealthModule::register_services(framework::ServiceManager& manager) {
    // El módulo de salud no tiene servicios que registrar por ahora.
}

std::vector<std::unique_ptr<framework::IRoute>> HealthModule::get_route_handlers() {
    auto routes = std::vector<std::unique_ptr<framework::IRoute>>();
    routes.push_back(std::make_unique<HealthRoutes>());
    return routes;
}

std::string HealthModule::get_prefix() const {
    return "/health";
}

} // namespace grabanzo::mserver::modules::health

REGISTER_MODULE("health",
                HealthModule,
                grabanzo::mserver::modules::health::HealthModule);
