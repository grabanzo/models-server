#pragma once

#include <string>

#include <grabanzo/mserver/module.hpp>

namespace grabanzo::mserver {

class ModuleBase : public Module
{
  public:
    ModuleBase(const std::string name)
      : name_{ name }
    {
    }

    ~ModuleBase() override = default;

    std::string getName() const override { return name_; };

  private:
    const std::string name_;
};

} // namespace grabanzo::mserver::framework
