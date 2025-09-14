#pragma once

#include <grabanzo/mserver/module_base.hpp>
#include <grabanzo/mserver/modules/health/health_module_config.hpp>
#include <grabanzo/mserver/route.hpp>

namespace grabanzo::mserver::modules {

class HealthModule : public grabanzo::mserver::ModuleBase
{
  public:
    HealthModule(const HealthModuleConfig config)
      : grabanzo::mserver::ModuleBase("health")
      , health_module_config_{config}
    {
    }
    std::vector<std::unique_ptr<grabanzo::mserver::Route>> getRouteHandlers()
      override;
    std::string getPrefix() const override;

  private:
    const HealthModuleConfig health_module_config_;
};

} // namespace grabanzo::mserver::modules::health
