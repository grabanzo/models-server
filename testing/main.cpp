#include <type_traits>
#include <concepts>
#include <string>
#include <iostream>
#include <utility>


class IModule {

};

class IConfig {
public:
    std::string name;
};

template<typename TConfig>
concept IsIConfig = std::is_base_of<IConfig, TConfig>::value;

template<typename TConfig>
requires IsIConfig<TConfig>
class ModuleBase : public IModule {
public:
    ModuleBase(TConfig config) : config_{std::move(config)} {};
    void show_me() {
        std::cout << "config name:" << config_.name << '\n';
    }
protected:
    TConfig config_;
};

class ModuleAConfig : public IConfig {
public:
    int value;
};

class ModuleA : public ModuleBase<ModuleAConfig> {
public:
    using ModuleBase<ModuleAConfig>::ModuleBase;
    void show();
};

void ModuleA::show() {
    std::cout << "config " << config_.name << '\n';
    std::cout << "config " << config_.value << '\n';

}
int main(int argc, char* argv[]) {
    ModuleAConfig c = {"name", 0};
    ModuleA m = ModuleA(c);
    m.show_me();
}