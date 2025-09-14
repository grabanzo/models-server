#pragma once

#include <string>

#include <cxxopts.hpp>
#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/config.hpp>
#include <grabanzo/mserver/thread_pool_config.hpp>

namespace grabanzo::mserver {

class WorkerHttpConfig : public Configuration
{
  public:
    using Configuration::Configuration;
    WorkerHttpConfig(const YAML::Node& root_node);

    std::string host = "127.0.0.1";
    int port = 8080;
    int maxRequests = 0;
    int shutdownTimeoutSec = 10;
    ThreadPoolConfig threadPool;
};

class WorkerHttpArgs : public Args
{
  public:
    using Args::Args;
    WorkerHttpArgs(cxxopts::Options& options);
    void set_result(cxxopts::ParseResult& result);
};
} // namespace grabanzo::mserver::http
