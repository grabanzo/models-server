#include <httplib.h>
#include <nlohmann/json.hpp>

#include <grabanzo/mserver/modules/health/health_routes.hpp>

namespace grabanzo::mserver::modules {

void
HealthRoutes::registerRoutes(httplib::Server& server,
                             const std::string& prefix,
                             RouteContext& context)
{
    server.Get(
      prefix + "/",
      [this, &context](const httplib::Request& req, httplib::Response& res) {
          handleHealth(req, res, context);
      });
}

void
HealthRoutes::handleHealth(const httplib::Request& req,
                           httplib::Response& res,
                           RouteContext& context)
{
    nlohmann::json response_json;
    response_json["status"] = "ok";
    response_json["module"] = "dummy";
    response_json["message_from_service"] = health_module_config_.log_message;
    res.set_content(response_json.dump(), "application/json");
}

} // namespace grabanzo::mserver::modules::health
