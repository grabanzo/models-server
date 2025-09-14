#pragma once

#include <httplib.h>

#include <grabanzo/mserver/route.hpp>
#include <grabanzo/mserver/modules/dummy/dummy_module_config.hpp>

// Reemplaza 'Dummy' por el nombre de tu módulo.
namespace grabanzo::mserver::modules {

class DummyRoutes : public grabanzo::mserver::Route {
public:
    DummyRoutes() = default;
    DummyRoutes(const DummyModuleConfig config) : dummy_module_config_{config} {};

    void registerRoutes(httplib::Server& server, const std::string& prefix, grabanzo::mserver::RouteContext& context) override;

private:
    /**
     * @brief Manejador para la ruta de ejemplo.
     */
    void handleDummy(const httplib::Request &req, httplib::Response &res, grabanzo::mserver::RouteContext& context);
    const DummyModuleConfig dummy_module_config_;
};

} // namespace grabanzo::mserver::modules::dummy
