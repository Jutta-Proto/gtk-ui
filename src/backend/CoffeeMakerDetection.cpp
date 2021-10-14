#include "CoffeeMakerDetection.hpp"
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
CoffeeMakerDetection::CoffeeMakerDetection(std::string&& btName) : btName(std::move(btName)) {
    disp.connect(sigc::mem_fun(*this, &CoffeeMakerDetection::on_notification_from_worker_thread));
}

CoffeeMakerDetection::~CoffeeMakerDetection() {
    stop();
    try_join();
}

void CoffeeMakerDetection::set_state(CoffeeMakerDetectionState newState) {
    if (newState != state) {
        state = newState;
        disp.emit();
    }
}

void CoffeeMakerDetection::start() {
    try_join();
    assert(!mainThread);
    SPDLOG_DEBUG("Starting coffee maker detection...");
    canceled = false;
    mainThread = std::make_optional<std::thread>(&CoffeeMakerDetection::run, this);
}

void CoffeeMakerDetection::try_join() {
    if (mainThread) {
        mainThread->join();
        mainThread = std::nullopt;
    }
}

void CoffeeMakerDetection::stop() {
    if (!mainThread) {
        return;
    }
    SPDLOG_DEBUG("Stoping coffee maker detection...");
    canceled = true;
    if (state == CoffeeMakerDetectionState::RUNNING) {
        set_state(CoffeeMakerDetectionState::CANCELD);
    }
}

void CoffeeMakerDetection::run() {
    set_state(CoffeeMakerDetectionState::RUNNING);
    SPDLOG_DEBUG("Coffee maker detection started.");
    while (state == CoffeeMakerDetectionState::RUNNING) {
        try {
            std::shared_ptr<bt::ScanArgs> result = bt::scan_for_device(std::string{btName}, &canceled);
            if (!result) {
                SPDLOG_INFO("No coffee maker found. Sleeping...");
                std::this_thread::sleep_for(std::chrono::milliseconds{500});
                continue;
            }
            SPDLOG_INFO("Coffee maker found.");
            coffeeMaker = std::make_shared<jutta_bt_proto::CoffeeMaker>(std::string{result->name}, std::string{result->addr});
            set_state(CoffeeMakerDetectionState::SUCCESS);
            return;
        } catch (const std::exception& ex) {
            lastError = std::string(ex.what());
            SPDLOG_ERROR("Failed to discover BT devices with: {}", lastError);
            set_state(CoffeeMakerDetectionState::ERROR);
            return;
        }
    }
    set_state(CoffeeMakerDetectionState::NOT_RUNNING);
    SPDLOG_INFO("Coffee maker detection got canceled.");
}

CoffeeMakerDetection::CoffeeMakerDetectionState CoffeeMakerDetection::get_state() const {
    return state;
}

std::shared_ptr<jutta_bt_proto::CoffeeMaker>&& CoffeeMakerDetection::get_coffee_maker() {
    return std::move(coffeeMaker);
}

const std::string& CoffeeMakerDetection::get_last_error() const {
    return lastError;
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
