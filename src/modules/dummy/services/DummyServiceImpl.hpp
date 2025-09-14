#pragma once

#include "IDummyService.hpp"

namespace grabanzo::mserver::modules::dummy::services {

/**
 * @class DummyServiceImpl
 * @brief Implementación concreta del servicio de plantilla.
 *
 * Esta clase contiene la lógica real del servicio.
 */
class DummyServiceImpl : public IDummyService {
public:
    DummyServiceImpl() = default;

    std::string get_message() const override;
};

} // namespace grabanzo::mserver::modules::dummy::services
