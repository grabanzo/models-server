#include <memory>

#include <grabanzo/mserver/framework/ModuleRegistry.hpp>
#include <grabanzo/mserver/framework/ServiceManager.hpp>

#include "DummyModule.hpp"
#include "routes/DummyRoutes.hpp"
#include "services/IDummyService.hpp"
#include "services/DummyServiceImpl.hpp"

// Reemplaza 'Dummy' por el nombre de tu módulo.
namespace grabanzo::mserver::modules::dummy {

// Si tu módulo no necesita registrar servicios, puedes dejar esta función vacía.
void DummyModule::register_services(framework::ServiceManager& manager) {
    // Registra la implementación del servicio con una clave única.
    // La clave permite a otros componentes solicitar este servicio.
    // Es importante registrarlo usando la interfaz (IDummyService)
    // para que los consumidores no dependan de la implementación concreta.
    manager.register_service<services::IDummyService>(
        std::make_shared<services::DummyServiceImpl>()
    );
}

std::vector<std::unique_ptr<framework::IRoute>> DummyModule::get_route_handlers() {
    auto routes = std::vector<std::unique_ptr<framework::IRoute>>();
    // Añade aquí todas las clases de rutas que tenga tu módulo.
    routes.push_back(std::make_unique<DummyRoutes>());
    return routes;
}

std::string DummyModule::get_prefix() const {
    // Define aquí el prefijo de la URL para tu módulo.
    return "/dummy";
}

} // namespace grabanzo::mserver::modules::dummy

// --- Registro Automático del Módulo ---
// Reemplaza "dummy" por el nombre de tu módulo y ajusta el nombre de la clase.
// El primer parámetro es un nombre único en minúsculas.
// El segundo es un nombre de variable único (puedes usar el nombre del módulo).
// El tercero es el nombre completo de la clase del módulo con su namespace.
REGISTER_MODULE("dummy",
                DummyModule,
                grabanzo::mserver::modules::dummy::DummyModule);
