#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

#include <grabanzo/mserver/core/Logger.hpp>
#include <grabanzo/mserver/framework/ServiceManager.hpp>
#include <grabanzo/mserver/framework/IModule.hpp>
#include <grabanzo/mserver/framework/ModuleRegistry.hpp>
#include <grabanzo/mserver/http/HttpServer.hpp>
#include <grabanzo/mserver/http/HttpServerConfig.hpp>
#include "MasterProcess.hpp"
#include "AppConfig.hpp"

namespace grabanzo::mserver {

// Inicialización de los miembros estáticos
std::atomic<bool> MasterProcess::shutdown_request_ = false;
std::atomic<bool> MasterProcess::child_terminated_ = false;

MasterProcess::MasterProcess(AppConfig& config) : config_(config) {}

void MasterProcess::run() {
    LOG_INFO("Master process starting up...");
    LOG_INFO("  - Host: {}", config_.httpServer.host);
    LOG_INFO("  - Port: {}", config_.httpServer.port);
    LOG_INFO("  - Worker processes: {}", config_.workers);
    LOG_INFO("  - Threads per worker: {}", config_.httpServer.threadPoolSize);

    setup_signal_handlers();
    spawn_initial_workers();
    supervision_loop();
    shutdown_workers();

    LOG_INFO("Master process shut down gracefully.");
}

void MasterProcess::setup_signal_handlers() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGCHLD, signal_handler);
}

void MasterProcess::signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        shutdown_request_ = true;
    } else if (signum == SIGCHLD) {
        child_terminated_ = true;
    }
}

void MasterProcess::spawn_initial_workers() {
    for (size_t i = 0; i < config_.workers; ++i) {
        spawn_worker();
    }
}

void MasterProcess::spawn_worker() {
    pid_t pid = fork();
    if (pid == 0) { // Proceso hijo
        run_worker(config_);
        exit(1); // No debería llegar aquí
    } else if (pid > 0) { // Proceso padre
        worker_pids_.push_back(pid);
        LOG_INFO("Started worker process with PID {}.", pid);
    } else {
        LOG_CRITICAL("Failed to fork a new worker process. This is a fatal error. Shutting down master.");
        // Si fork() falla, el master no puede continuar. Termina.
        exit(1);
    }
}

void MasterProcess::run_worker(AppConfig& config) {
    LOG_INFO("Worker process started (PID: {})", getpid());

    // El worker no debe manejar las señales del master (INT, TERM, CHLD)
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
    
    try {
        // --- Creación del ServiceManager y Registro de Servicios ---
        auto service_manager = std::make_shared<framework::ServiceManager>();
        
        // --- Carga de Módulos ---
        auto modules = framework::ModuleRegistry::instance().create_all_modules();
        
        // 3. Crear el servidor pasándole el service_manager y los módulos.
        http::HttpServer server(config.httpServer, service_manager, modules);
        server.run();
    } catch (const std::exception& e) {
        LOG_CRITICAL("Unhandled exception in worker process {}: {}", getpid(), e.what());
        exit(EXIT_FAILURE);
    }

    LOG_INFO("Worker process (PID: {}) shutting down cleanly.", getpid());
    exit(EXIT_SUCCESS);
}

void MasterProcess::supervision_loop() {
    while (!shutdown_request_) {
        // Pausa el proceso master hasta que llegue una señal.
        pause();

        if (child_terminated_) {
            int status;
            pid_t dead_pid;
            // Cosecha todos los hijos que hayan terminado
            while ((dead_pid = waitpid(-1, &status, WNOHANG)) > 0) {
                LOG_WARN("Worker process with PID {} died.", dead_pid);
                worker_pids_.erase(std::remove(worker_pids_.begin(), worker_pids_.end(), dead_pid), worker_pids_.end());
                
                // Si no es un apagado programado, lo reiniciamos
                if (!shutdown_request_) {
                    respawn_worker();
                }
            }
            child_terminated_ = false; // Resetea el flag
        }
    }
}

void MasterProcess::respawn_worker() {
    LOG_INFO("Restarting worker process...");
    spawn_worker();
}

void MasterProcess::shutdown_workers() {
    LOG_INFO("Shutdown requested. Terminating all worker processes...");
    for (pid_t pid : worker_pids_) {
        kill(pid, SIGTERM);
    }
    // Espera a que todos los hijos terminen
    while(wait(nullptr) > 0);
}

} // namespace grabanzo::mserver
