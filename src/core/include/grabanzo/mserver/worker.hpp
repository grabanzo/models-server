#pragma once

#include <memory>

namespace grabanzo::mserver {

class Worker
{
  public:
    Worker() = default;
    virtual ~Worker() = default;

    int start();

  protected:
    virtual int run() = 0;
    virtual void stop() = 0;

  private:
    std::atomic<bool> shutdown_initiated_{ false };

    static void graceful_shutdown_handler(int signum);
    static std::atomic<Worker*> running_server_instance_;
};

class WorkerFactory
{
  public:
    WorkerFactory() = default;
    virtual ~WorkerFactory() = default;
    virtual std::unique_ptr<Worker> createWorker() = 0;
};

}
