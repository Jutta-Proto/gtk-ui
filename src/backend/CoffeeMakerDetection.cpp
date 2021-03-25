#include "CoffeeMakerDetection.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <cassert>
#include <logger/Logger.hpp>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>

namespace backend {
CoffeeMakerDetection::CoffeeMakerDetection(std::string&& device) : connection(std::make_unique<jutta_proto::JuttaConnection>(std::move(device))) {
    disp.connect(sigc::mem_fun(*this, &CoffeeMakerDetection::on_notification_from_worker_thread));
}

CoffeeMakerDetection::~CoffeeMakerDetection() {
    if (state == CoffeeMakerDetectionState::RUNNING) {
        stop();
    }
}

void CoffeeMakerDetection::set_state(CoffeeMakerDetectionState newState) {
    if (newState != state) {
        state = newState;
        disp.emit();
    }
}

void CoffeeMakerDetection::start() {
    assert(!mainThread);
    SPDLOG_DEBUG("Starting coffee maker detection...");
    mainThread = std::make_optional<std::thread>(&CoffeeMakerDetection::run, this);
}

void CoffeeMakerDetection::stop() {
    assert(mainThread);
    SPDLOG_DEBUG("Stoping coffee maker detection...");
    set_state(CoffeeMakerDetectionState::CANCELD);
    mainThread->join();
    SPDLOG_DEBUG("Stoped coffee maker detection.");
}

void CoffeeMakerDetection::run() {
    SPDLOG_DEBUG("Coffee maker detection started.");
    try {
        connection->init();
    } catch (const std::exception& ex) {
        lastError = std::string(ex.what());
        SPDLOG_ERROR("Failed to initialize the coffee maker connection with: {}", lastError);
        set_state(CoffeeMakerDetectionState::ERROR);
    }

    std::vector<uint8_t> readBuffer{};
    while (state == CoffeeMakerDetectionState::RUNNING) {
        try {
            static_cast<void>(connection->read_decoded(readBuffer));
            if (!readBuffer.empty()) {
                std::string resultRead = jutta_proto::JuttaConnection::vec_to_string(readBuffer);
                if (resultRead.starts_with("ty:") && resultRead.ends_with("\r\n")) {
                    if (state == CoffeeMakerDetectionState::RUNNING) {
                        // Remove 'ty:' and '\r\n':
                        std::string version = resultRead.substr(3, resultRead.length() - 3 - 2);
                        SPDLOG_INFO("Successfully found the coffee maker: {}", version);
                        set_state(CoffeeMakerDetectionState::SUCCESS);
                        return;
                    }
                }
                readBuffer.clear();
            }
        } catch (const std::exception& ex) {
            lastError = std::string(ex.what());
            SPDLOG_ERROR("Failed to read from the serial connection with: {}", lastError);
            set_state(CoffeeMakerDetectionState::ERROR);
            return;
        }
    }
    SPDLOG_INFO("Coffee maker detection got canceled.");
}

CoffeeMakerDetection::CoffeeMakerDetectionState CoffeeMakerDetection::get_state() const {
    return state;
}

std::unique_ptr<jutta_proto::JuttaConnection>&& CoffeeMakerDetection::get_connection() {
    return std::move(connection);
}

const std::string& CoffeeMakerDetection::get_last_error() const {
    return lastError;
}

CoffeeMakerDetection::type_signal_state_changed CoffeeMakerDetection::signal_state_changed() {
    return m_signal_state_changed;
}

//-----------------------------Events:-----------------------------
void CoffeeMakerDetection::on_notification_from_worker_thread() {
    // Emit the signal handler:
    m_signal_state_changed.emit(state);
}
}  // namespace backend
