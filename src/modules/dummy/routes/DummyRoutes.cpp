#include <nlohmann/json.hpp>

#include <grabanzo/mserver/core/Logger.hpp>
#include <grabanzo/mserver/framework/RouteContext.hpp>
#include <grabanzo/mserver/framework/ServiceManager.hpp>
#include <grabanzo/mserver/framework/IConfigService.hpp> // <-- Incluir el nuevo servicio

#include "DummyRoutes.hpp"
#include "services/IDummyService.hpp"
#include "../DummyModuleConfig.hpp" // <-- Incluir la struct de config

// Reemplaza 'Dummy' por el nombre de tu módulo.
namespace grabanzo::mserver::modules::dummy {

using namespace grabanzo::mserver;

void DummyRoutes::register_routes(httplib::Server& server, const std::string& prefix, framework::RouteContext& context) {
    // Registra una ruta GET en la raíz del prefijo del módulo (ej. /dummy/).
    // La ruta completa se construye concatenando el prefijo del módulo con la ruta relativa.
    server.Get(prefix + "/", [this, &context](const httplib::Request &req, httplib::Response &res) {
        this->handle_dummy(req, res, context);
    });

    // Puedes añadir más rutas aquí...
    // server.Post(prefix + "/otro-endpoint", ...);
}

void DummyRoutes::handle_dummy(const httplib::Request &req, httplib::Response &res, framework::RouteContext& context) {
    // Lógica del manejador de la ruta.
    LOG_INFO("Petición recibida en la ruta del módulo de plantilla.");

    // 1. Obtener el servicio de configuración.
    auto config_service = context.service_manager.get_service<framework::IConfigService>();

    // 2. Obtener la configuración tipada para este módulo.
    auto module_config = config_service->get_module_config<dummy::DummyModuleConfig>("dummy");

    // 3. Obtener el servicio Dummy.
    auto dummy_service = context.service_manager.get_service<services::IDummyService>();

    std::string message;
    if (dummy_service) {
        // 4. Usar la configuración para influir en el servicio.
        message = module_config.log_message + " (Intentos: " + std::to_string(module_config.retries) + ")";
    } else {
        message = "DummyService no está disponible.";
        LOG_ERROR("No se pudo obtener 'IDummyService'. ¿Está registrado correctamente?");
    }

    // 5. Devolver una respuesta JSON.
    nlohmann::json response_json;
    response_json["status"] = "ok";
    response_json["module"] = "dummy";
    response_json["message_from_service"] = message;

    res.set_content(response_json.dump(), "application/json");
}

} // namespace grabanzo::mserver::modules::dummy
