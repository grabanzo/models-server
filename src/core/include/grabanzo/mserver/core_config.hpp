#pragma once

#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/app_config.hpp>
#include <grabanzo/mserver/worker_http_config.hpp>

namespace grabanzo::mserver {

class CoreConfig
{
  public:
    CoreConfig(const YAML::Node& root_node)
      : app_config{ root_node }
      , worker_http_config{ root_node } {};

    AppConfig app_config;
    WorkerHttpConfig worker_http_config;
};

class CoreArgs
{
  public:
    CoreArgs();
  
    void parse(int argc, char* argv[]);
    std::string config_file;

  protected:
    void setResults(cxxopts::ParseResult& result);
    virtual void setDerivedResults(cxxopts::ParseResult& result){};
    cxxopts::Options options;
    WorkerHttpArgs worker_http_args;
    AppArgs app_args;
};

YAML::Node
LoadConfig(const std::string& config_file);

template<class TCoreConfig>
concept IsCoreConfig = std::is_base_of<CoreConfig, TCoreConfig>::value;

template<class TCoreArgs>
concept IsCoreArgs = std::is_base_of<CoreArgs, TCoreArgs>::value;

} // namespace grabanzo::mserver
