#pragma once

#include <grabanzo/mserver/route.hpp>
#include <grabanzo/mserver/modules/health/health_module_config.hpp>

namespace grabanzo::mserver::modules {

class HealthRoutes : public grabanzo::mserver::Route {
public:
    HealthRoutes() = default;
    HealthRoutes(const HealthModuleConfig config) : health_module_config_{config} {};

    void registerRoutes(httplib::Server& server, const std::string& prefix, grabanzo::mserver::RouteContext& context) override;

private:
    void handleHealth(const httplib::Request &req, httplib::Response &res, grabanzo::mserver::RouteContext& context);
    const HealthModuleConfig health_module_config_;

};

} // namespace grabanzo::mserver::modules::health
