#pragma once

#include <string>

#include <grabanzo/mserver/http/HttpServerConfig.hpp>

namespace grabanzo::mserver {

/**
 * @struct AppConfig
 * @brief Almacena todos los parámetros de configuración de la aplicación.
 */
struct AppConfig {
    int workers;
    std::string logLevel;
    http::HttpServerConfig httpServer;
};

} // namespace grabanzo::mserver
