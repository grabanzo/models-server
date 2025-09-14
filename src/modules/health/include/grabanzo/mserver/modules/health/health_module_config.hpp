#pragma once

#include <string>

#include <cxxopts.hpp>
#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/config.hpp>

namespace grabanzo::mserver::modules {

class HealthModuleConfig : public grabanzo::mserver::Configuration
{
  public:
    HealthModuleConfig()
      : Configuration() {};
    HealthModuleConfig(const YAML::Node& root_node)
      : grabanzo::mserver::Configuration()
    {
        if (root_node["health"]) {
            auto node = root_node["health"];
            if (node["log_message"])
                log_message = node["log_message"].as<std::string>(log_message);
        }
    }

    std::string log_message = "Mensaje por defecto del servicio Health!";
};

} // namespace grabanzo::mserver::modules::dummy
