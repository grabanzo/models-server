#include <grabanzo/mserver/worker.hpp>

namespace grabanzo::mserver {

std::atomic<Worker*> Worker::running_server_instance_ = nullptr;

void
Worker::graceful_shutdown_handler(int signum)
{
    if (auto* httpServerInstance = running_server_instance_.load()) {
        if (!httpServerInstance->shutdown_initiated_.exchange(true)) {
            httpServerInstance->stop();
        }
    }
}

int
Worker::start()
{
    running_server_instance_ = this;

    signal(SIGINT, graceful_shutdown_handler);
    signal(SIGTERM, graceful_shutdown_handler);

    return run();
}
}
