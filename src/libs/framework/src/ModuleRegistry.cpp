#include <grabanzo/mserver/framework/ModuleRegistry.hpp>

namespace grabanzo::mserver::framework {

ModuleRegistry& ModuleRegistry::instance() {
    static ModuleRegistry reg;
    return reg;
}

void ModuleRegistry::register_module(const std::string& name, ModuleFactory factory) {
    factories_[name] = factory;
}

std::shared_ptr<IModule> ModuleRegistry::create_module(const std::string& name) {
    auto it = factories_.find(name);
    if (it != factories_.end()) {
        return it->second();
    }
    return nullptr; // O lanzar una excepción
}

std::vector<std::shared_ptr<IModule>> ModuleRegistry::create_all_modules() {
    std::vector<std::shared_ptr<IModule>> modules;
    for (const auto& pair : factories_) {
        modules.push_back(pair.second());
    }
    return modules;
}

ModuleRegistrar::ModuleRegistrar(const std::string& name, ModuleFactory factory) {
    ModuleRegistry::instance().register_module(name, factory);
}

} // namespace grabanzo::mserver::framework
