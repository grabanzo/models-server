#pragma once

#include <grabanzo/mserver/route.hpp>

namespace grabanzo::mserver {

class Module
{
  public:
    Module() = default;
    virtual ~Module() = default;

    virtual std::vector<std::unique_ptr<Route>> getRouteHandlers() = 0;
    virtual std::string getPrefix() const = 0;
    virtual std::string getName() const = 0;
};
} // namespace grabanzo::mserver
