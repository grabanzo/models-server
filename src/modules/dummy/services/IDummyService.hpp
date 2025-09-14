#pragma once

#include <string>

#include <grabanzo/mserver/framework/IService.hpp>

namespace grabanzo::mserver::modules::dummy::services {

/**
 * @class IDummyService
 * @brief Interfaz de ejemplo para un servicio.
 *
 * Define el "contrato" que debe cumplir cualquier implementación de este servicio.
 * Las rutas y otros componentes dependerán de esta interfaz, no de la clase concreta.
 */
class IDummyService : public framework::IService {
public:
    virtual ~IDummyService() = default;

    /**
     * @brief Realiza una operación de ejemplo.
     * @return Un mensaje de ejemplo.
     */
    virtual std::string get_message() const = 0;
};

} // namespace grabanzo::mserver::modules::dummy::services
