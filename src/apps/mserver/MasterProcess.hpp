#pragma once

#include <vector>
#include <atomic>
#include <sys/types.h> // for pid_t
#include <unistd.h>

#include "AppConfig.hpp"

namespace grabanzo::mserver {

/**
 * @class MasterProcess
 * @brief Orquesta el ciclo de vida de la aplicación.
 *
 * Se encarga de arrancar, supervisar y terminar los procesos worker.
 */
class MasterProcess {
public:
    /**
     * @brief Constructor del proceso maestro.
     * @param config La configuración de la aplicación ya cargada.
     */
    explicit MasterProcess(AppConfig& config);

    /**
     * @brief Inicia el bucle principal del proceso maestro.
     * Esta función bloquea la ejecución y solo retorna cuando el servidor se apaga.
     */
    void run();

private:
    void setup_signal_handlers();
    void spawn_initial_workers();
    void supervision_loop();
    void respawn_worker();
    void shutdown_workers();
    void spawn_worker();

    // Este método estático se ejecuta en el proceso hijo
    static void run_worker(AppConfig& config);

    // Manejador de señales estático
    static void signal_handler(int signum);

    // Flags estáticos para la comunicación con el manejador de señales
    static std::atomic<bool> shutdown_request_;
    static std::atomic<bool> child_terminated_;

    AppConfig& config_;
    std::vector<pid_t> worker_pids_;
};

} // namespace grabanzo::mserver
