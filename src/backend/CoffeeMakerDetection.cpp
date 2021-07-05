#include "CoffeeMakerDetection.hpp"
#include "jutta_proto/JuttaCommands.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <cassert>
#include <chrono>
#include <logger/Logger.hpp>
#include <memory>
#include <optional>
#include <thread>
#include <spdlog/spdlog.h>
#include <sys/stat.h>

namespace backend {
CoffeeMakerDetection::CoffeeMakerDetection(std::string&& device) : connection(std::make_unique<jutta_proto::JuttaConnection>(std::move(device))) {
    disp.connect(sigc::mem_fun(*this, &CoffeeMakerDetection::on_notification_from_worker_thread));
}

CoffeeMakerDetection::~CoffeeMakerDetection() {
    stop();
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
    if (!mainThread) {
        return;
    }
    SPDLOG_DEBUG("Stoping coffee maker detection...");
    if (state == CoffeeMakerDetectionState::RUNNING) {
        set_state(CoffeeMakerDetectionState::CANCELD);
    }
    mainThread->join();
    mainThread = std::nullopt;
    SPDLOG_DEBUG("Stoped coffee maker detection.");
    set_state(CoffeeMakerDetectionState::NOT_RUNNING);
}

void CoffeeMakerDetection::run() {
    set_state(CoffeeMakerDetectionState::RUNNING);
    SPDLOG_DEBUG("Coffee maker detection started.");
    try {
        connection->init();
    } catch (const std::exception& ex) {
        lastError = std::string(ex.what());
        SPDLOG_ERROR("Failed to initialize the coffee maker connection with: {}", lastError);
        set_state(CoffeeMakerDetectionState::ERROR);
        return;
    }

    std::vector<uint8_t> readBuffer{};
    std::shared_ptr<std::string> buffer = nullptr;
    while (state == CoffeeMakerDetectionState::RUNNING) {
        try {
            buffer = connection->write_decoded_with_response("TY:\r\n", std::chrono::milliseconds{1000});
            if (!buffer) {
                SPDLOG_WARN("Failed to read/write to coffee maker to get type...");
                std::this_thread::sleep_for(std::chrono::milliseconds{500});
                continue;
            }

            if (starts_with(*buffer, "ty:") && ends_with(*buffer, "\r\n")) {
                if (state == CoffeeMakerDetectionState::RUNNING) {
                    // Remove 'ty:' and '\r\n':
                    version = buffer->substr(3, buffer->length() - 3 - 2);
                    SPDLOG_INFO("Successfully found the coffee maker: {}", version);
                    set_state(CoffeeMakerDetectionState::SUCCESS);
                    return;
                }
            } else {
                SPDLOG_DEBUG("Invalid string read: '{}' with starts_with: '{}', ends_with: '{}'", *buffer, starts_with(*buffer, "ty:"), ends_with(*buffer, "\r\n"));
                std::this_thread::sleep_for(std::chrono::milliseconds{500});
                continue;
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

const std::string& CoffeeMakerDetection::get_version() const {
    return version;
}

CoffeeMakerDetection::type_signal_state_changed CoffeeMakerDetection::signal_state_changed() {
    return m_signal_state_changed;
}

bool CoffeeMakerDetection::starts_with(const std::string& s, const std::string& prefix) {
    if (s.length() < prefix.length()) {
        return false;
    }
    for (size_t i = 0; i < prefix.length(); i++) {
        if (s[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

bool CoffeeMakerDetection::ends_with(const std::string& s, const std::string& postfix) {
    if (s.length() < postfix.length()) {
        return false;
    }
    for (size_t i = 0; i < postfix.length(); i++) {
        if (s[s.length() - postfix.length() + i] != postfix[i]) {
            return false;
        }
    }
    return true;
}

//-----------------------------Events:-----------------------------
void CoffeeMakerDetection::on_notification_from_worker_thread() {
    // Emit the signal handler:
    m_signal_state_changed.emit(state);
}
}  // namespace backend
