#pragma once

#include <memory>

namespace grabanzo::mserver::framework {

/**
 * @class IService
 * @brief Interfaz base para todas las clases de servicio.
 *
 * Esta clase actúa como un marcador. Permite que los servicios
 * se gestionen de forma polimórfica en un ServiceManager.
 */
class IService {
public:
    virtual ~IService() = default;
};

} // namespace grabanzo::mserver::framework
