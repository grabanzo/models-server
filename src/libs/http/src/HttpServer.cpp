#include <vector>
#include <string>
#include <atomic> // Added for atomic server instance
#include <csignal> // Added for signal handling

#include <grabanzo/mserver/core/Logger.hpp>
#include <grabanzo/mserver/framework/RouteContext.hpp>
#include <grabanzo/mserver/framework/IModule.hpp>

#include <grabanzo/mserver/http/HttpServer.hpp>

namespace grabanzo::mserver::http {

using namespace grabanzo::mserver;

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
      max_requests_(config.maxRequests),
      shutdown_timeout_sec_(config.shutdownTimeoutSec),
      host(config.host),
      port(config.port),
      route_context_{
          .thread_pool = thread_pool,
          .service_manager = *service_manager_,
          .pre_request_handler = [this](){ this->handle_request(); }
      }
{
    LOG_INFO("Configuring server from {} modules...", modules.size());

    // El servidor itera sobre los módulos para configurarse a sí mismo.
    for (const auto& module : modules) {
        // 1. Les pide que registren sus servicios en el manager central.
        module->register_services(*service_manager_);

        // 2. Les pide sus manejadores de rutas y los añade a la lista del servidor.
        auto module_routes = module->get_route_handlers();
        routes_.insert(routes_.end(),
                       std::make_move_iterator(module_routes.begin()),
                       std::make_move_iterator(module_routes.end()));
    }

    if (max_requests_ > 0) {
        LOG_INFO("HttpServer created. Worker will restart after {} requests.", max_requests_);
    } else {
        LOG_INFO("HttpServer created. Worker request limit is disabled.");
    }
    LOG_INFO("Graceful shutdown timeout set to {} seconds.", shutdown_timeout_sec_);

    // --- Registramos las Rutas de cada Módulo ---
    // Iteramos sobre los módulos para registrar sus rutas, usando el `route_context_` del miembro de la clase.
    for (const auto& module : modules) {
        auto prefix = module->get_prefix();
        auto route_handlers = module->get_route_handlers();
        for (const auto& route_handler : route_handlers) {
            route_handler->register_routes(server, prefix, route_context_);
        }
    }
    LOG_INFO("All routes configured.");
}

void HttpServer::handle_request() {
    if (max_requests_ == 0) {
        return; // Límite desactivado
    }

    const size_t count = request_count_.fetch_add(1, std::memory_order_relaxed) + 1;
    
    if (count >= max_requests_) {
        LOG_WARN("Request limit ({}) reached. Worker will shut down after this request.", max_requests_);
        server.stop();
    }
}

void HttpServer::run() {
    // 1. Registrar esta instancia para el manejador de señales
    running_server_instance_ = this;

    // 2. Configurar los manejadores de señales para este worker
    signal(SIGINT, HttpServer::graceful_shutdown_handler);
    signal(SIGTERM, HttpServer::graceful_shutdown_handler);

    // Establecemos el timeout de lectura. Durante el apagado, esto actúa como
    // el tiempo máximo que se esperará a que una conexión activa termine.
    server.set_read_timeout(shutdown_timeout_sec_, 0);

    // Habilitamos SO_REUSEPORT para permitir que múltiples procesos escuchen en el mismo puerto.
    // Esto es clave para nuestra arquitectura pre-fork.
    server.set_socket_options([](socket_t sock) {
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const void *>(&opt), sizeof(opt));
    });

    LOG_INFO("Server worker listening on {}:{}", host, port);
    if (!server.listen(host.c_str(), port)) {
        LOG_CRITICAL("Failed to start server on {}:{}. Shutting down worker.", host, port);
        running_server_instance_ = nullptr; // Limpiar antes de salir
        exit(1);
    }

    // 3. Limpiar el puntero al finalizar
    running_server_instance_ = nullptr;
}

} // namespace grabanzo::mserver::http
