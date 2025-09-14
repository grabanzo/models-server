#pragma once

#include <grabanzo/mserver/module_base.hpp>

#include <grabanzo/mserver/modules/dummy/dummy_module_config.hpp>

namespace grabanzo::mserver::modules {

class DummyModule : public grabanzo::mserver::ModuleBase {
public:
    explicit DummyModule(const DummyModuleConfig config)
      : ModuleBase("dummy"), dummy_module_config_{config}
    {
    }

    // void register_services(grabanzo::mserver::ServiceManager& manager) override;

    std::vector<std::unique_ptr<grabanzo::mserver::Route>> getRouteHandlers() override;
    std::string getPrefix() const override;
private:
    const DummyModuleConfig dummy_module_config_;
};

} // namespace grabanzo::mserver::modules::dummy
