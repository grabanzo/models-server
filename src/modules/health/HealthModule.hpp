#pragma once

#include <grabanzo/mserver/framework/IModule.hpp>

namespace grabanzo::mserver::modules::health {

/**
 * @class HealthModule
 * @brief Módulo que encapsula la funcionalidad de health-check.
 *
 * Este módulo no registra servicios, solo provee las rutas para el endpoint /health.
 */
class HealthModule : public framework::IModule {
public:
    void register_services(framework::ServiceManager& manager) override;
    std::vector<std::unique_ptr<framework::IRoute>> get_route_handlers() override;
    std::string get_prefix() const override;
};

} // namespace grabanzo::mserver::modules::health
