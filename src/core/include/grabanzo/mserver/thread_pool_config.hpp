#pragma once

#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/config.hpp>

namespace grabanzo::mserver {

class ThreadPoolConfig : public Configuration
{
  public:
    using Configuration::Configuration;
    ThreadPoolConfig(const YAML::Node& root_node);

    int size;
};

} // namespace grabanzo::mserver::http
