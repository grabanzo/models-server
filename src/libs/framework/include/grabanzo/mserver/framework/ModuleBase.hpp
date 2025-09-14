#pragma once

#include <string>

#include <grabanzo/mserver/framework/IModule.hpp>

namespace grabanzo::mserver::framework {

class ModuleBase : public IModule {
public:
    ModuleBase(const std::string name)  : name_(name) {}

    virtual ~ModuleBase() = default;

    std::string get_name() const override { return name_; } ;

private:
    std::string name_;
};

} // namespace grabanzo::mserver::framework
