#pragma once

#include <jutta_proto/CoffeeMaker.hpp>
#include <jutta_proto/JuttaConnection.hpp>
#include <memory>

namespace backend {
class CoffeeMakerWrapper {
 private:
    const std::string version;
    const std::unique_ptr<jutta_proto::CoffeeMaker> coffeeMaker;

 public:
    CoffeeMakerWrapper(std::string&& version, std::unique_ptr<jutta_proto::JuttaConnection>&& connection);

    const std::string& get_version();
    const std::unique_ptr<jutta_proto::CoffeeMaker>& get_coffee_maker();
};
}  // namespace backend
