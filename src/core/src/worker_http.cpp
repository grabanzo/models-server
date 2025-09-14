#include <csignal>

#include <grabanzo/mserver/module.hpp>
#include <grabanzo/mserver/thread_pool.hpp>
#include <grabanzo/mserver/worker_http.hpp>
#include <grabanzo/mserver/worker_http_config.hpp>
#include <grabanzo/mserver/logger.hpp>

namespace grabanzo::mserver {

WorkerHttp::WorkerHttp(const WorkerHttpConfig config,
                       std::vector<std::unique_ptr<Module>> modules)
  : worker_http_config_(config)
  , thread_pool_(worker_http_config_.threadPool)
  , modules_{ std::move(modules) }
  , route_context_{ .thread_pool = thread_pool_ }
{
    server_.set_pre_request_handler(
      [this](const httplib::Request&, httplib::Response&) {
          if (worker_http_config_.maxRequests > 0) {
              const size_t count =
                request_count_.fetch_add(1, std::memory_order_relaxed) + 1;
              if (count >= worker_http_config_.maxRequests) {
                  server_.stop();
              }
          }
          return httplib::Server::HandlerResponse::Handled;
      });

    for (const auto& module : modules_) {
        auto prefix = module->getPrefix();
        auto route_handlers = module->getRouteHandlers();
        for (const auto& route_handler : route_handlers) {
            route_handler->registerRoutes(server_, prefix, route_context_);
        }
    }
}

int
WorkerHttp::run()
{
    // Establecemos el timeout de lectura. Durante el apagado, esto actúa como
    // el tiempo máximo que se esperará a que una conexión activa termine.
    server_.set_read_timeout(worker_http_config_.shutdownTimeoutSec, 0);

    // Habilitamos SO_REUSEPORT para permitir que múltiples procesos escuchen en
    // el mismo puerto. Esto es clave para nuestra arquitectura pre-fork.
    server_.set_socket_options([](socket_t sock) {
        int opt = 1;
        setsockopt(sock,
                   SOL_SOCKET,
                   SO_REUSEPORT,
                   reinterpret_cast<const void*>(&opt),
                   sizeof(opt));
    });

    LOG_INFO("Host Port {}:{}", worker_http_config_.host, worker_http_config_.port);
    if (!server_.listen(worker_http_config_.host.c_str(), worker_http_config_.port)) {
        exit(1);
    }
    return 0;
}

void
WorkerHttp::stop()
{
    server_.stop();
}

std::unique_ptr<Worker>
WorkerHttpFactory::createWorker()
{
    std::vector<std::unique_ptr<Module>> modules = createModules();
    return std::make_unique<WorkerHttp>(worker_http_config_, std::move(modules));
}

} // namespace grabanzo::mserver
