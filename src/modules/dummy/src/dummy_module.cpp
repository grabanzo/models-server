#include <memory>

#include <grabanzo/mserver/modules/dummy/dummy_routes.hpp>
#include <grabanzo/mserver/modules/dummy/dummy_module.hpp>

namespace grabanzo::mserver::modules {

std::vector<std::unique_ptr<grabanzo::mserver::Route>> DummyModule::getRouteHandlers() {
    auto routes = std::vector<std::unique_ptr<grabanzo::mserver::Route>>();
    routes.push_back(std::make_unique<DummyRoutes>(dummy_module_config_));
    return routes;
}

std::string DummyModule::getPrefix() const {
    // Define aquí el prefijo de la URL para tu módulo.
    return "/dummy";
}

} // namespace grabanzo::mserver::modules::dummy
