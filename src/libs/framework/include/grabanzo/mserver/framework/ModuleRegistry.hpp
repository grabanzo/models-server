#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <grabanzo/mserver/framework/IModule.hpp>

namespace grabanzo::mserver::framework {

// Un "factory" es una función que sabe cómo crear un objeto.
using ModuleFactory = std::function<std::shared_ptr<IModule>()>;

/**
 * @class ModuleRegistry
 * @brief Singleton que administra la creación de instancias de módulos.
 *
 * Este registro mantiene una colección de fábricas de módulos,
 * permitiendo que los módulos se descubran y se instancien dinámicamente.
 */
class ModuleRegistry {
public:
    static ModuleRegistry& instance();

    void register_module(const std::string& name, ModuleFactory factory);
    std::shared_ptr<IModule> create_module(const std::string& name);
    std::vector<std::shared_ptr<IModule>> create_all_modules();

private:
    ModuleRegistry() = default;
    std::unordered_map<std::string, ModuleFactory> factories_;
};

/**
 * @class ModuleRegistrar
 * @brief Ayudante para registrar módulos de forma automática al inicio del programa.
 *
 * Este ayudante debe ser instanciado en el punto de inicio de la aplicación,
 * generalmente en la función main().
 */
class ModuleRegistrar {
public:
    ModuleRegistrar(const std::string& name, ModuleFactory factory);
};

} // namespace grabanzo::mserver::framework

#define REGISTER_MODULE(name_str, simple_class_name, full_class_name) \
    static grabanzo::mserver::framework::ModuleRegistrar registrar_##simple_class_name( \
        name_str, []() { return std::make_shared<full_class_name>(); });
