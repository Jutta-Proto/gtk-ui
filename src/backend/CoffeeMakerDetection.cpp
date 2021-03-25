#include "CoffeeMakerDetection.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <cassert>
#include <logger/Logger.hpp>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>

namespace backend {
CoffeeMakerDetection::CoffeeMakerDetection(std::string&& device) : connection(std::make_unique<jutta_proto::JuttaConnection>(std::move(device))) {}

void CoffeeMakerDetection::start() {
    assert(!mainThread);
    SPDLOG_DEBUG("Starting coffee maker detection...");
    shouldRun = true;
    isRunning = true;
    mainThread = std::make_optional<std::thread>(&CoffeeMakerDetection::run, this);
}

void CoffeeMakerDetection::stop() {
    assert(mainThread);
    SPDLOG_DEBUG("Stoping coffee maker detection...");
    shouldRun = false;
    mainThread->join();
    SPDLOG_DEBUG("Stoped coffee maker detection.");
}

void CoffeeMakerDetection::run() {
    SPDLOG_DEBUG("Coffee maker detection started.");
    try {
        connection->init();
    } catch (const std::exception& ex) {
        isRunning = false;
        shouldRun = false;
        success = false;
        SPDLOG_ERROR("Failed to initialize the coffee maker connection with: {}", ex.what());
    }

    std::vector<uint8_t> readBuffer{};
    while (shouldRun) {
        static_cast<void>(connection->read_decoded(readBuffer));
        if (!readBuffer.empty()) {
            std::string resultRead = jutta_proto::JuttaConnection::vec_to_string(readBuffer);
            if (resultRead.starts_with("ty:") && resultRead.ends_with("\r\n")) {
                // Remove 'ty:' and '\r\n':
                std::string version = resultRead.substr(3, resultRead.length() - 3 - 2);
                SPDLOG_INFO("Successfully found the coffee maker: {}", version);
                success = true;
                isRunning = false;
                return;
            }
            readBuffer.clear();
        }
    }
    SPDLOG_INFO("Coffee maker detection got canceled.");
    isRunning = false;
    success = false;
}

bool CoffeeMakerDetection::was_successfull() const {
    return success;
}

bool CoffeeMakerDetection::is_running() const {
    return isRunning;
}

std::unique_ptr<jutta_proto::JuttaConnection>&& CoffeeMakerDetection::get_connection() {
    return std::move(connection);
}
}  // namespace backend
