#include <cstdlib>
#include <string>
#include <stdlib.h>

#include <yaml-cpp/yaml.h>
#include <cxxopts.hpp>

#include <grabanzo/mserver/config.hpp>
#include <grabanzo/mserver/logger.hpp>
#include <grabanzo/mserver/thread_pool_config.hpp>
#include <grabanzo/mserver/worker_http_config.hpp>

namespace grabanzo::mserver {

WorkerHttpConfig::WorkerHttpConfig(const YAML::Node& root_node)
  : Configuration()
{
    if (root_node["http"]) {
        auto node = root_node["http"];
        if (node["host"])
            host = node["host"].as<std::string>(host);
        if (node["port"])
            port = node["port"].as<int>(port);
        if (node["maxRequests"])
            maxRequests = node["maxRequests"].as<int>(maxRequests);
        if (node["shutdownTimeoutSec"])
            shutdownTimeoutSec =
            node["shutdownTimeoutSec"].as<int>(shutdownTimeoutSec);
    }
    port = getenv<int>("MSERVER_HTTP_PORT", port);
    host = getenv<std::string>("MSERVER_HTTP_HOST", host);
    threadPool = ThreadPoolConfig(root_node["http"]);
}

WorkerHttpArgs::WorkerHttpArgs(cxxopts::Options& options) : Args(options) {
    options.add_options("worker-http")
        ("p,port", "Port", cxxopts::value<int>())
        ("b,host", "Host", cxxopts::value<std::string>())
        ;
}

void WorkerHttpArgs::set_result(cxxopts::ParseResult& result) {
    setenv<int>("MSERVER_HTTP_PORT", "port", result);
    setenv<std::string>("MSERVER_HTTP_HOST", "host", result);
}

} // namespace grabanzo::mserver
