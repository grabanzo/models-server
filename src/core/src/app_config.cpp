#include <stdexcept>
#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/app_config.hpp>

namespace grabanzo::mserver {

AppConfig::AppConfig(const YAML::Node& root_node) : Configuration()
{
    if (root_node["app"]) {
        auto app_node = root_node["app"];
        if (app_node["workers"])
            workers =
              app_node["workers"].as<int>(workers);
        if (app_node["logLevel"])
            logLevel =
              app_node["logLevel"].as<std::string>(logLevel);
    }

    if (workers <= 0) {
        throw std::runtime_error("The number of workers must be positive.");
    }

}

} // namespace grabanzo::mserver::core
