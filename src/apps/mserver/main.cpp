#include <filesystem>
#include <iostream>
#include <figcone/configreader.h>
#include <figcone/figcone.h>
#include <grabanzo/mserver/core/Logger.hpp>

#include "MasterProcess.hpp"
#include "AppConfig.hpp"


int main(int argc, char* argv[]) {
    grabanzo::mserver::core::Log::Init();

    auto cfgReader = figcone::ConfigReader{};
    
    auto cfg = cfgReader.readYamlFile<grabanzo::mserver::AppConfig>(std::filesystem::canonical("config/config.yaml"));
    
    grabanzo::mserver::MasterProcess master(cfg);
    master.run();
    
    return 0;
}