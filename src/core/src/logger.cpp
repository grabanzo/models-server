#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/pattern_formatter.h>

#include <grabanzo/mserver/logger.hpp>

namespace grabanzo::mserver {

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init() {
    // Creamos un "sink" para la consola con colores
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // auto formatter = std::make_unique<spdlog::pattern_formatter>(
    //     R"([%f] [%l] [%n] %v)"
    // );
    // auto formatter = std::make_unique<spdlog::pattern_formatter>(
    //     R"({ "timestamp": "%Y-%m-%dT%H:%M:%S.%e%z", "level": "%l", "process_id": %P, "thread_id": %t, "message": "%v" })",
    //     spdlog::pattern_time_type::utc
    // );
    // console_sink->set_formatter(std::move(formatter));
    console_sink->set_pattern("[%f] [%P/%t] [%^%l%$] %v");

    // Creamos el logger con el sink que acabamos de configurar
    s_Logger = std::make_shared<spdlog::logger>("ModelsServerLogger", console_sink);
    
    // Lo registramos para que esté disponible globalmente
    spdlog::register_logger(s_Logger);
    
    // Establecemos el nivel de log por defecto (se pueden registrar todos los niveles)
    s_Logger->set_level(spdlog::level::trace);
    s_Logger->flush_on(spdlog::level::trace);
}

void Log::Shutdown() {
    s_Logger.reset();
    spdlog::drop_all();
}

} // namespace grabanzo::mserver::core
