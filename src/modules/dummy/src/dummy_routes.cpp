#include <nlohmann/json.hpp>

#include <grabanzo/mserver/modules/dummy/dummy_routes.hpp>

namespace grabanzo::mserver::modules {

void
DummyRoutes::registerRoutes(httplib::Server& server,
                             const std::string& prefix,
                             grabanzo::mserver::RouteContext& context)
{
    // Registra una ruta GET en la raíz del prefijo del módulo (ej. /dummy/).
    // La ruta completa se construye concatenando el prefijo del módulo con la
    // ruta relativa.
    server.Get(
      prefix + "/",
      [this, &context](const httplib::Request& req, httplib::Response& res) {
          this->handleDummy(req, res, context);
      });

    // Puedes añadir más rutas aquí...
    // server.Post(prefix + "/otro-endpoint", ...);
}

void
DummyRoutes::handleDummy(const httplib::Request& req,
                          httplib::Response& res,
                          grabanzo::mserver::RouteContext& context)
{
    // // 1. Obtener el servicio de configuración.
    // auto config_service = context.service_manager.get_service<DummyService>();

    // // 3. Obtener el servicio Dummy.
    // auto dummy_service =
    //   context.service_manager.get_service<DummyService>();

    std::string message;
    // if (dummy_service) {
        // 4. Usar la configuración para influir en el servicio.
        message = dummy_module_config_.log_message +
                  " (Intentos: " + std::to_string(dummy_module_config_.retries) + ")";
    // } else {
    //     message = "DummyService no está disponible.";
    // }

    // 5. Devolver una respuesta JSON.
    nlohmann::json response_json;
    response_json["status"] = "ok";
    response_json["module"] = "dummy";
    response_json["message_from_service"] = message;

    res.set_content(response_json.dump(), "application/json");
}

} // namespace grabanzo::mserver::modules::dummy
