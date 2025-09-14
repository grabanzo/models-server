#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <stdexcept>

#include <grabanzo/mserver/framework/IService.hpp>

namespace grabanzo::mserver::framework {

/**
 * @class ServiceManager
 * @brief Un contenedor y proveedor de servicios basado en tipos.
 *
 * Esta clase actúa como un registro central para todas las implementaciones de IService.
 * Permite registrar servicios y luego recuperarlos de forma segura utilizando su tipo.
 * Esto desacopla el código que utiliza los servicios (como las rutas) de las
 * implementaciones concretas.
 */
class ServiceManager {
public:
    /**
     * @brief Registra un servicio en el manager.
     * @tparam T El tipo concreto del servicio (debe heredar de IService).
     * @param service Un shared_ptr al servicio a registrar.
     */
    template <typename T>
    void register_service(std::shared_ptr<T> service) {
        static_assert(std::is_base_of<IService, T>::value, "El servicio debe heredar de IService");
        auto type_index = std::type_index(typeid(T));
        services_[type_index] = service;
    }

    /**
     * @brief Obtiene un servicio del manager por su tipo.
     * @tparam T El tipo del servicio a obtener.
     * @return std::shared_ptr<T> Un puntero al servicio solicitado.
     * @throws std::runtime_error si el servicio no está registrado.
     */
    template <typename T>
    std::shared_ptr<T> get_service() const {
        static_assert(std::is_base_of<IService, T>::value, "El servicio debe heredar de IService");
        auto type_index = std::type_index(typeid(T));
        auto it = services_.find(type_index);
        if (it == services_.end()) {
            throw std::runtime_error("Servicio no encontrado: " + std::string(typeid(T).name()));
        }
        return std::static_pointer_cast<T>(it->second);
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<IService>> services_;
};

} // namespace grabanzo::mserver::framework
