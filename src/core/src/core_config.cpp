#include "grabanzo/mserver/core_config.hpp"
#include "grabanzo/mserver/worker_http_config.hpp"
#include <cxxopts.hpp>
#include <dotenv.h>
#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/config.hpp>

namespace grabanzo::mserver {

YAML::Node
LoadConfig(const std::string& config_file)
{
    return YAML::LoadFile(config_file);
}

CoreArgs::CoreArgs()
  : options{ "core", "Models Server" }
{
    options.add_options()("c,config",
                          "Config file path",
                          cxxopts::value<std::string>()->default_value(
                            "config/config.yaml"))("h,help", "Print usage");
    worker_http_args = WorkerHttpArgs(options);
    app_args = AppArgs(options);
}

void
CoreArgs::parse(int argc, char* argv[])
{
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    config_file = result["config"].as<std::string>();
    setResults(result);
}

void
CoreArgs::setResults(cxxopts::ParseResult& result)
{
    worker_http_args.set_result(result);
    app_args.set_result(result);
    setDerivedResults(result);
}
}
