#include <algorithm>
#include <csignal>
#include <unistd.h>

#include <grabanzo/mserver/app.hpp>
#include <grabanzo/mserver/logger.hpp>

namespace grabanzo::mserver {

std::unique_ptr<App> App::app_;
std::atomic<bool> App::shutdown_request_ = false;

int
App::start()
{
    try {
        setupApp();
        loop();
        shutdown();
        Log::Shutdown();
    } catch (...) {
        return -1;
    }
    return 0;
}

void
App::loop()
{
    while (!shutdown_request_) {
        int status;
        pid_t dead_pid;
        while ((dead_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            worker_pids_.erase(
              std::remove(worker_pids_.begin(), worker_pids_.end(), dead_pid),
              worker_pids_.end());
        }

        size_t required_workers = config_.workers;
        if (worker_pids_.size() < required_workers) {
            for (size_t i = worker_pids_.size(); i < required_workers; ++i) {
                spawnWorker();
            }
        }

        sleep(1);
    }
}

void
App::shutdown()
{
    for (pid_t pid : worker_pids_) {
        kill(pid, SIGTERM);
    }
    // Espera a que todos los hijos terminen
    while (wait(nullptr) > 0)
        ;
}

void
App::setupApp()
{
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGCHLD, sigHandler);
}

void
App::spawnWorker()
{
    pid_t pid = fork();
    if (pid == 0) { // Proceso hijo
        startWorker();
        exit(1);          // No debería llegar aquí
    } else if (pid > 0) { // Proceso padre
        worker_pids_.push_back(pid);
    } else {
        // Si fork() falla, el master no puede continuar. Termina.
        exit(1);
    }
}

void
App::startWorker()
{
    // El worker no debe manejar las señales del master (INT, TERM, CHLD)
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);

    try {
        auto worker = worker_factory_->createWorker();
        worker->start();
    } catch (const std::exception& e) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void
App::sigHandler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM) {
        shutdown_request_ = true;
    }
}

}
