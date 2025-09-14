#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/config.hpp>

namespace grabanzo::mserver {

class AppConfig : public Configuration
{
  public:
    using Configuration::Configuration;
    AppConfig(const YAML::Node& root_node);

    int workers = 4;
    std::string logLevel = "info";
};

class AppArgs : public Args
{
  public:
    using Args::Args;
    AppArgs(cxxopts::Options& options)
      : Args(options) {};
    void set_result(cxxopts::ParseResult& result) {};
};

} // namespace grabanzo::mserver
