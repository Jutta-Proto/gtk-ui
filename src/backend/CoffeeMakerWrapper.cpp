#include "CoffeeMakerWrapper.hpp"
#include "jutta_proto/CoffeeMaker.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <memory>

namespace backend {
CoffeeMakerWrapper::CoffeeMakerWrapper(std::string&& version, std::unique_ptr<jutta_proto::JuttaConnection>&& connection) : version(std::move(version)),
                                                                                                                            coffeeMaker(std::make_unique<jutta_proto::CoffeeMaker>(std::move(connection))) {}

const std::string& CoffeeMakerWrapper::get_version() {
    return version;
}

const std::unique_ptr<jutta_proto::CoffeeMaker>& CoffeeMakerWrapper::get_coffee_maker() {
    return coffeeMaker;
}

}  // namespace backend
