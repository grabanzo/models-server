#include <vector>
#include <string>
#include <atomic> 
#include <csignal>

#include <grabanzo/mserver/core/Logger.hpp>
#include <grabanzo/mserver/framework/RouteContext.hpp>
#include <grabanzo/mserver/framework/IModule.hpp>

#include <grabanzo/mserver/http/HttpServer.hpp>

namespace grabanzo::mserver::http {

// Variable estática para la instancia del servidor
std::atomic<HttpServer*> HttpServer::running_server_instance_ = nullptr;

// Manejador de señales global para el apagado ordenado.
// Se coloca en un namespace anónimo para limitar su alcance a este fichero.
void HttpServer::graceful_shutdown_handler(int signum) {
    if (auto* httpServerInstance = running_server_instance_.load()) {
        if (!httpServerInstance->shutdown_initiated_.exchange(true)) {
            LOG_WARN("Signal {} received. Shutting down worker gracefully...", signum);
            httpServerInstance->server.stop();
        }
    }
}

HttpServer::HttpServer(const HttpServerConfig& config,
                       std::shared_ptr<framework::ServiceManager> service_manager,
                       const std::vector<std::shared_ptr<framework::IModule>>& modules)
    : thread_pool(config.threadPoolSize),
      service_manager_(service_manager),
      config_(config),
      route_context_{
          .service_manager = *service_manager_,
          .thread_pool = thread_pool
      }
{
    LOG_INFO("Configuring server from {} modules...", modules.size());

    server.set_pre_request_handler([this](const httplib::Request&, httplib::Response&) {
        if (config_.maxRequests > 0) {
            const size_t count = request_count_.fetch_add(1, std::memory_order_relaxed) + 1;
            if (count >= config_.maxRequests) {
                LOG_WARN("Request limit ({}) reached. Worker will shut down after this request.", config_.maxRequests);
                server.stop();
            }
        }
        return httplib::Server::HandlerResponse::Handled;
    });

    // El servidor itera sobre los módulos para configurarse a sí mismo.
    for (const auto& module : modules) {
        LOG_INFO("Register service module {}.", module->get_name());
        // 1. Les pide que registren sus servicios en el manager central.
        module->register_services(*service_manager_);

        auto prefix = module->get_prefix();
        auto route_handlers = module->get_route_handlers();
        for (const auto& route_handler : route_handlers) {
            LOG_INFO("Register route {}.", prefix);
            route_handler->register_routes(server, prefix, route_context_);
        }
        LOG_INFO("All routes configured.");
    }

    if (config_.maxRequests > 0) {
        LOG_INFO("HttpServer created. Worker will restart after {} requests.", config_.maxRequests);
    } else {
        LOG_INFO("HttpServer created. Worker request limit is disabled.");
    }
    LOG_INFO("Graceful shutdown timeout set to {} seconds.", config_.shutdownTimeoutSec);
}

void HttpServer::run() {
    // 1. Registrar esta instancia para el manejador de señales
    running_server_instance_ = this;

    // 2. Configurar los manejadores de señales para este worker
    signal(SIGINT, HttpServer::graceful_shutdown_handler);
    signal(SIGTERM, HttpServer::graceful_shutdown_handler);

    // Establecemos el timeout de lectura. Durante el apagado, esto actúa como
    // el tiempo máximo que se esperará a que una conexión activa termine.
    server.set_read_timeout(config_.shutdownTimeoutSec, 0);

    // Habilitamos SO_REUSEPORT para permitir que múltiples procesos escuchen en el mismo puerto.
    // Esto es clave para nuestra arquitectura pre-fork.
    server.set_socket_options([](socket_t sock) {
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const void *>(&opt), sizeof(opt));
    });

    LOG_INFO("Server worker listening on {}:{}", config_.host, config_.port);
    if (!server.listen(config_.host.c_str(), config_.port)) {
        LOG_CRITICAL("Failed to start server on {}:{}. Shutting down worker.", config_.host, config_.port);
        running_server_instance_ = nullptr; // Limpiar antes de salir
        exit(1);
    }

    // 3. Limpiar el puntero al finalizar
    running_server_instance_ = nullptr;
}

} // namespace grabanzo::mserver::http
