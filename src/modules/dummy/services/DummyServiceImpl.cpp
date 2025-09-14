#include "DummyServiceImpl.hpp"

namespace grabanzo::mserver::modules::dummy::services {

std::string DummyServiceImpl::get_message() const {
    return "Hola desde DummyServiceImpl!";
}

} // namespace grabanzo::mserver::modules::dummy::services
