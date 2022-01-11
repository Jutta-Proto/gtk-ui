#include "CoffeeMakerConnectionHandler.hpp"
#include <bt/BLEDevice.hpp>
#include <bt/BLEHelper.hpp>
#include <cassert>
#include <chrono>
#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <logger/Logger.hpp>
#include <memory>
#include <optional>
#include <thread>
#include <spdlog/spdlog.h>
#include <sys/stat.h>

namespace backend {
CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandler() {
    disp.connect(sigc::mem_fun(*this, &CoffeeMakerConnectionHandler::on_notification_from_worker_thread));
    start();
}

CoffeeMakerConnectionHandler::~CoffeeMakerConnectionHandler() {
    stop();
    try_join();
}

void CoffeeMakerConnectionHandler::set_state(CoffeeMakerConnectionHandlerState newState) {
    if (newState != state) {
        state = newState;
        disp.emit();
    }
}

void CoffeeMakerConnectionHandler::start() {
    try_join();
    assert(!mainThread);
    SPDLOG_DEBUG("Starting coffee maker connection...");
    canceled = false;
    mainThread = std::make_optional<std::thread>(&CoffeeMakerConnectionHandler::run, this);
}

void CoffeeMakerConnectionHandler::try_join() {
    if (mainThread) {
        mainThread->join();
        mainThread = std::nullopt;
    }
}

void CoffeeMakerConnectionHandler::stop() {
    if (!mainThread) {
        return;
    }
    SPDLOG_DEBUG("Stoping coffee maker connection...");
    canceled = true;
}

// NOLINTNEXTLINE (readability-function-cognitive-complexity)
void CoffeeMakerConnectionHandler::run() {
    SPDLOG_DEBUG("Coffee maker connection started.");
    std::shared_ptr<bt::ScanArgs> scanResult = nullptr;

    while (!canceled) {
        // Cleanup:
        if (state != CoffeeMakerConnectionHandlerState::CONNECTED && coffeeMaker) {
            coffeeMaker->disconnect();
            coffeeMaker = nullptr;
            SPDLOG_DEBUG("Coffee maker connection: Cleanup...");
        }

        if (state == CoffeeMakerConnectionHandlerState::CONNECTED) {
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            continue;
        }

        if (state == CoffeeMakerConnectionHandlerState::NOT_RUNNING) {
            if (coffeeMaker) {
                coffeeMaker->disconnect();
                coffeeMaker = nullptr;
                SPDLOG_DEBUG("Coffee maker connection: Cleanup...");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            continue;
        }

        try {
            // Searching:
            if (state == CoffeeMakerConnectionHandlerState::SEARCHING) {
                assert(!btName.empty());

                SPDLOG_DEBUG("Coffee maker connection: Scanning...");
                scanResult = bt::scan_for_device(std::string{btName}, &canceled);
                if (scanResult) {
                    set_state(CoffeeMakerConnectionHandlerState::CONNECTING);
                } else {
                    SPDLOG_INFO("No coffee maker found. Sleeping...");
                    std::this_thread::sleep_for(std::chrono::milliseconds{500});
                }
            }

            // Connecting:
            else if (state == CoffeeMakerConnectionHandlerState::CONNECTING) {
                assert(scanResult);

                SPDLOG_DEBUG("Coffee maker connection: Connecting...");
                coffeeMaker = std::make_shared<jutta_bt_proto::CoffeeMaker>(std::string{scanResult->name}, std::string{scanResult->addr});
                coffeeMaker->set_state_changed_event_handler([this](const jutta_bt_proto::CoffeeMakerState& state) { this->on_coffee_maker_state_changed(state); });
                if (coffeeMaker->connect()) {
                    set_state(CoffeeMakerConnectionHandlerState::CONNECTED);
                } else {
                    coffeeMaker = nullptr;
                    SPDLOG_INFO("Failed to connect to coffee maker. Sleeping...");
                    std::this_thread::sleep_for(std::chrono::milliseconds{500});
                }
            }
        } catch (const std::exception& ex) {
            lastError = std::string(ex.what());
            set_state(CoffeeMakerConnectionHandlerState::SEARCHING);
            SPDLOG_ERROR("BT connection failed with: {}\nSleeping...", lastError);

            // Cleanup:
            if (scanResult) {
                scanResult = nullptr;
            }
            if (coffeeMaker) {
                coffeeMaker->disconnect();
                coffeeMaker = nullptr;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }
    }
    SPDLOG_INFO("Coffee maker connection got canceled.");
}

void CoffeeMakerConnectionHandler::set_name(std::string&& btName) {
    this->btName = std::move(btName);
    if (state != CoffeeMakerConnectionHandlerState::NOT_RUNNING) {
        set_state(CoffeeMakerConnectionHandlerState::CONNECTING);
    }
}

void CoffeeMakerConnectionHandler::connect() {
    assert(state == CoffeeMakerConnectionHandlerState::NOT_RUNNING);
    set_state(CoffeeMakerConnectionHandlerState::SEARCHING);
}

void CoffeeMakerConnectionHandler::disconnect() {
    set_state(CoffeeMakerConnectionHandlerState::NOT_RUNNING);
}

CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState CoffeeMakerConnectionHandler::get_state() const {
    return state;
}

std::shared_ptr<jutta_bt_proto::CoffeeMaker>&& CoffeeMakerConnectionHandler::get_coffee_maker() {
    return std::move(coffeeMaker);
}

const std::string& CoffeeMakerConnectionHandler::get_last_error() const {
    return lastError;
}

CoffeeMakerConnectionHandler::type_signal_state_changed CoffeeMakerConnectionHandler::signal_state_changed() {
    return m_signal_state_changed;
}

bool CoffeeMakerConnectionHandler::starts_with(const std::string& s, const std::string& prefix) {
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

bool CoffeeMakerConnectionHandler::ends_with(const std::string& s, const std::string& postfix) {
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

CoffeeMakerConnectionHandler& CoffeeMakerConnectionHandler::get_instance() {
    static CoffeeMakerConnectionHandler instance;
    return instance;
}

//-----------------------------Events:-----------------------------
void CoffeeMakerConnectionHandler::on_notification_from_worker_thread() {
    // Emit the signal handler:
    m_signal_state_changed.emit(state);
}

void CoffeeMakerConnectionHandler::on_coffee_maker_state_changed(const jutta_bt_proto::CoffeeMakerState& state) {
    if (state == jutta_bt_proto::CoffeeMakerState::DISCONNECTING || state == jutta_bt_proto::CoffeeMakerState::DISCONNECTED) {
        if (this->state != CoffeeMakerConnectionHandlerState::NOT_RUNNING) {
            set_state(CoffeeMakerConnectionHandlerState::SEARCHING);
        }
    }
}
}  // namespace backend
