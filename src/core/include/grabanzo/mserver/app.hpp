#pragma once

#include <memory>
#include <unistd.h>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <grabanzo/mserver/app_config.hpp>
#include <grabanzo/mserver/core_config.hpp>
#include <grabanzo/mserver/logger.hpp>
#include <grabanzo/mserver/worker.hpp>

namespace grabanzo::mserver {

class App final
{
  public:
    template<typename TWorkerFactory, typename TCoreConfig, typename TCoreArgs>
        requires IsCoreConfig<TCoreConfig> && IsCoreArgs<TCoreArgs>
    friend App* MainApp(int argc, char* argv[]);

    int start();

  private:
    App(const CoreConfig config, std::unique_ptr<WorkerFactory> worker_factory)
      : config_{ config.app_config }
      , worker_factory_{ std::move(worker_factory) } {};

    void setupApp();
    void loop();
    void shutdown();
    void spawnWorker();
    void startWorker();

    static void sigHandler(int signum);

    const AppConfig config_;
    std::vector<pid_t> worker_pids_;
    std::unique_ptr<WorkerFactory> worker_factory_;

    static std::atomic<bool> shutdown_request_;
    static std::unique_ptr<App> app_;
};

template<typename TWorkerFactory, typename TCoreConfig, typename TCoreArgs>
    requires IsCoreConfig<TCoreConfig> && IsCoreArgs<TCoreArgs>
App*
MainApp(int argc, char* argv[])
{
    if (App::app_)
        return App::app_.get();

    TCoreArgs core_args = TCoreArgs();
    core_args.parse(argc, argv);
    Log::Init();
    YAML::Node root_node = LoadConfig(core_args.config_file);
    TCoreConfig config = TCoreConfig(root_node);
    auto worker_factory = std::make_unique<TWorkerFactory>(config);
    App::app_ =
      std::unique_ptr<App>(new App(config, std::move(worker_factory)));
    return App::app_.get();
}

} // namespace grabanzo::mserver
