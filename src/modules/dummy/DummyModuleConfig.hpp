#pragma once
#include <string>
#include <yaml-cpp/yaml.h>

namespace grabanzo::mserver::modules::dummy {

/**
 * @struct DummyModuleConfig
 * @brief Define la estructura de configuración para el DummyModule.
 *
 * Esta struct agrupa todas las opciones de configuración para el módulo,
 * permitiendo un acceso fuertemente tipado y proporcionando valores por defecto.
 */
struct DummyModuleConfig {
    std::string log_message = "Mensaje por defecto del servicio Dummy!";
    int retries = 3;
};

} // namespace grabanzo::mserver::modules::dummy


namespace YAML {
/**
 * @brief Especialización de la plantilla de conversión de yaml-cpp para DummyModuleConfig.
 *
 * Esto le enseña a la librería cómo convertir un nodo YAML a nuestra struct,
 * permitiendo un parseo automático y seguro.
 */
template<>
struct convert<grabanzo::mserver::modules::dummy::DummyModuleConfig> {
    static bool decode(const Node& node, grabanzo::mserver::modules::dummy::DummyModuleConfig& rhs) {
        // Usamos el segundo parámetro de .as<T>() para proporcionar un valor por defecto
        // si la clave no existe en el fichero YAML. Aquí, usamos el valor que ya
        // tiene la struct (su valor por defecto) como fallback.
        rhs.log_message = node["log_message"].as<std::string>(rhs.log_message);
        rhs.retries = node["retries"].as<int>(rhs.retries);
        return true;
    }
};
} // namespace YAML
