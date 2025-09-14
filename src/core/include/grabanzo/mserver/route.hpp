#pragma once

#include <string>

#include <httplib.h>

#include <grabanzo/mserver/thread_pool.hpp>

namespace grabanzo::mserver {

struct RouteContext
{
    ThreadPool& thread_pool;
};

class Route
{
  public:
    virtual ~Route() = default;

    virtual void registerRoutes(httplib::Server& server,
                                 const std::string& prefix,
                                 RouteContext& context) = 0;
};

} // namespace grabanzo::mserver::framework
