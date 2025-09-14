#pragma once

#include <grabanzo/mserver/framework/IModule.hpp>
#include <grabanzo/mserver/framework/ServiceManager.hpp>

// Reemplaza 'Dummy' por el nombre de tu módulo.
namespace grabanzo::mserver::modules::dummy {

/**
 * @class DummyModule
 * @brief Implementación de la interfaz IModule para el módulo de plantilla.
 *
 * Esta clase sirve como punto de entrada para el módulo. Es responsable de:
 * - Registrar cualquier servicio específico del módulo en el ServiceManager.
 * - Proporcionar los manejadores de rutas (IRoute) que gestionan los endpoints HTTP.
 * - Definir el prefijo de URL base para todas las rutas del módulo (ej. "/dummy").
 */
class DummyModule : public framework::IModule {
public:
    /**
     * @brief Registra los servicios del módulo.
     * Si tu módulo no necesita servicios, deja esta función vacía.
     */
    void register_services(framework::ServiceManager& manager) override;

    /**
     * @brief Obtiene los manejadores de rutas del módulo.
     * @return Un vector de punteros únicos a las clases que implementan IRoute.
     */
    std::vector<std::unique_ptr<framework::IRoute>> get_route_handlers() override;

    /**
     * @brief Devuelve el prefijo de ruta para este módulo.
     * @return El prefijo de la URL (ej. "/dummy").
     */
    std::string get_prefix() const override;
};

} // namespace grabanzo::mserver::modules::dummy
