#pragma once

#include <cstddef>
#include <string>

namespace grabanzo::mserver::http {

/**
 * @struct HttpServerConfig
 * @brief Contiene los parámetros de configuración específicos para el HttpServer.
 *
 * Esta estructura separa la configuración del servidor web de la configuración
 * general de la aplicación.
 */
struct HttpServerConfig {
    std::string host;
    int port;
    int threadPoolSize;
    int maxRequests;
    int shutdownTimeoutSec;
};

} // namespace grabanzo::mserver::http
