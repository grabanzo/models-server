#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/config.hpp>

namespace grabanzo::mserver::modules {

/**
 * @struct DummyModuleConfig
 * @brief Define la estructura de configuración para el DummyModule.
 *
 * Esta struct agrupa todas las opciones de configuración para el módulo,
 * permitiendo un acceso fuertemente tipado y proporcionando valores por
 * defecto.
 */
class DummyModuleConfig : public grabanzo::mserver::Configuration
{
  public:
    DummyModuleConfig()
      : Configuration() {};
    DummyModuleConfig(const YAML::Node& root_node) : grabanzo::mserver::Configuration() {
        if (root_node["dummy"]) {
            auto dummy_node = root_node["dummy"];
            if(dummy_node["log_message"])
                log_message = dummy_node["log_message"].as<std::string>(log_message);
            if(dummy_node["retries"])
                retries = dummy_node["retries"].as<int>(retries);
        }
    }

    std::string log_message = "Mensaje por defecto del servicio Dummy!";
    int retries = 3;
};

} // namespace grabanzo::mserver::modules::dummy
