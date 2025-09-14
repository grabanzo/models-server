#pragma once

#include <spdlog/spdlog.h>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace grabanzo::mserver::core {

/**
 * @class Log
 * Proporciona una interfaz estática para el registro de mensajes en toda la aplicación.
 */
class Log {
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

    static void Shutdown();

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace grabanzo::mserver::core

// --- Macros de conveniencia ---
// Estas macros permiten llamar al logger de forma más concisa.
// Así podemos llamar a LOG_INFO("mensaje") en lugar de ModelsServer::core::Log::GetLogger()->info("mensaje")
#define LOG_INFO(...)    ::grabanzo::mserver::core::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)    ::grabanzo::mserver::core::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)   ::grabanzo::mserver::core::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::grabanzo::mserver::core::Log::GetLogger()->critical(__VA_ARGS__)
#define LOG_DEBUG(...)   ::grabanzo::mserver::core::Log::GetLogger()->debug(__VA_ARGS__)
