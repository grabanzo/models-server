#pragma once

#include <httplib.h>
#include <atomic>
#include <memory>
#include <string>
#include <vector>

// Add full definitions for classes used as members
#include <grabanzo/mserver/core/ThreadPool.hpp>
#include <grabanzo/mserver/framework/ServiceManager.hpp>
#include <grabanzo/mserver/framework/IRoute.hpp>
#include <grabanzo/mserver/framework/RouteContext.hpp>

#include <grabanzo/mserver/http/HttpServerConfig.hpp>

// Forward declarations for types used only in function signatures
namespace grabanzo::mserver::framework {
    class IService;
    class IModule;
}

namespace grabanzo::mserver::http {

class HttpServer {
public:
    /**
     * @brief Constructor del servidor HTTP.
     * 
     * @param config La configuración para esta instancia del servidor.
     * @param service_manager El manager con todos los servicios ya registrados.
     */
    HttpServer(const HttpServerConfig& config,
               std::shared_ptr<framework::ServiceManager> service_manager,
               const std::vector<std::shared_ptr<framework::IModule>>& modules);

    void run();

private:
    void handle_request();

    static void graceful_shutdown_handler(int signum);

    core::ThreadPool thread_pool;
    std::shared_ptr<framework::ServiceManager> service_manager_;
    httplib::Server server;
    framework::RouteContext route_context_;

    std::vector<std::unique_ptr<framework::IRoute>> routes_;

    const size_t max_requests_;
    const int shutdown_timeout_sec_;
    std::atomic<size_t> request_count_{0};
    std::atomic<bool> shutdown_initiated_{false};

    static std::atomic<HttpServer*> running_server_instance_;
    const std::string& host;
    const int port;
};

} // namespace grabanzo::mserver::http