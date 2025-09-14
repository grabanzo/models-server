#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/thread_pool_config.hpp>

namespace grabanzo::mserver {

ThreadPoolConfig::ThreadPoolConfig(const YAML::Node& root_node)
  : Configuration()
{
    if (root_node["threadPool"]) {
        auto http_node = root_node["threadPool"];
        if (http_node["size"])
            size = http_node["size"].as<int>(size);
    }
}

} // namespace grabanzo::mserver
