#include <vector>

#include <grabanzo/mserver/app.hpp>
#include <grabanzo/mserver/core_config.hpp>
#include <grabanzo/mserver/app_config.hpp>
#include <grabanzo/mserver/module.hpp>
#include <grabanzo/mserver/modules/health/health_module.hpp>
#include <grabanzo/mserver/modules/health/health_module_config.hpp>
#include <grabanzo/mserver/worker_http.hpp>
#include <grabanzo/mserver/worker_http_config.hpp>

namespace grabanzo::mserver {
class MServerCoreConfig : public CoreConfig
{
  public:
    MServerCoreConfig(const YAML::Node& root_node)
      : CoreConfig(root_node)
      , health{ root_node } {};

    modules::HealthModuleConfig health;
};

class WorkerMServerFactory : public WorkerHttpFactory
{
  public:
    using WorkerHttpFactory::WorkerHttpFactory;
    WorkerMServerFactory(const MServerCoreConfig& config)
      : WorkerHttpFactory(config)
      , health_module_config_{ config.health } {};

  protected:
    virtual std::vector<std::unique_ptr<Module>>
    createModules() override
    {
        std::vector<std::unique_ptr<Module>> modules;
        modules.push_back(
          std::make_unique<modules::HealthModule>(
            health_module_config_));
        return modules;
    };

    const modules::HealthModuleConfig health_module_config_;
};

}

int
main(int argc, char* argv[])
{
    using namespace grabanzo::mserver;
    auto app =
      grabanzo::mserver::MainApp<WorkerMServerFactory, MServerCoreConfig, CoreArgs>(argc, argv);
    app->start();
    return 0;
}