#pragma once

#include <jutta_proto/JuttaConnection.hpp>
#include <memory>
#include <optional>
#include <string>
#include <thread>

namespace backend {
class CoffeeMakerDetection {
 private:
    std::optional<std::thread> mainThread;
    bool shouldRun{false};
    bool isRunning{false};
    bool success{false};
    std::unique_ptr<jutta_proto::JuttaConnection> connection;

 public:
    explicit CoffeeMakerDetection(std::string&& device);

    void start();
    void stop();

    [[nodiscard]] bool was_successfull() const;
    [[nodiscard]] bool is_running() const;
    std::unique_ptr<jutta_proto::JuttaConnection>&& get_connection();

 private:
    void run();

    //-----------------------------Events:-----------------------------
};
}  // namespace backend
