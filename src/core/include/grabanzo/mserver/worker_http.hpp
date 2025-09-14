#pragma once

#include "grabanzo/mserver/core_config.hpp"
#include <atomic>
#include <memory>
#include <vector>

#include <httplib.h>

#include <grabanzo/mserver/worker_http_config.hpp>
#include <grabanzo/mserver/module.hpp>
#include <grabanzo/mserver/thread_pool.hpp>
#include <grabanzo/mserver/worker.hpp>

namespace grabanzo::mserver {

class WorkerHttp : public Worker
{
  public:
    WorkerHttp(const WorkerHttpConfig config,
               std::vector<std::unique_ptr<Module>> modules);

  protected:
    int run() override;
    void stop() override;
    const WorkerHttpConfig worker_http_config_;

  private:
    httplib::Server server_;
    ThreadPool thread_pool_;
    std::vector<std::unique_ptr<Module>> modules_;
    RouteContext route_context_;
    std::atomic<size_t> request_count_{ 0 };
};

class WorkerHttpFactory : public WorkerFactory
{
  public:
    WorkerHttpFactory(const CoreConfig& config)
      : WorkerFactory()
      , worker_http_config_{ config.worker_http_config } {};

    virtual std::unique_ptr<Worker> createWorker() override;

  protected:
    virtual std::vector<std::unique_ptr<Module>> createModules() = 0;
    const WorkerHttpConfig worker_http_config_;
};

} // namespace grabanzo::mserver
