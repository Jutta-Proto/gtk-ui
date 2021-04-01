#include "NfcDetection.hpp"
#include "jutta_proto/JuttaCommands.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <cassert>
#include <logger/Logger.hpp>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <sys/stat.h>

namespace backend {
NfcDetection::NfcDetection() {
    disp.connect(sigc::mem_fun(*this, &NfcDetection::on_notification_from_worker_thread));
}

NfcDetection::~NfcDetection() {
    stop();
}

void NfcDetection::set_state(NfcDetectionState newState) {
    if (newState != state) {
        state = newState;
        disp.emit();
    }
}

void NfcDetection::start() {
    assert(!mainThread);
    SPDLOG_DEBUG("Starting NFC card detection...");
    mainThread = std::make_optional<std::thread>(&NfcDetection::run, this);
}

void NfcDetection::stop() {
    if (!mainThread) {
        return;
    }
    SPDLOG_DEBUG("Stoping NFC card detection...");
    if (state == NfcDetectionState::RUNNING) {
        set_state(NfcDetectionState::CANCELD);
    }
    mainThread->join();
    mainThread = std::nullopt;
    SPDLOG_DEBUG("Stoped NFC card detection.");
    set_state(NfcDetectionState::NOT_RUNNING);
}

void NfcDetection::run() {
    set_state(NfcDetectionState::RUNNING);
    SPDLOG_DEBUG("NFC card detection started.");

    while (state == NfcDetectionState::RUNNING) {
        try {
        } catch (const std::exception& ex) {
            lastError = std::string(ex.what());
            SPDLOG_ERROR("NFC card detection failed with: {}", lastError);
            set_state(NfcDetectionState::ERROR);
            return;
        }
    }
    SPDLOG_INFO("NFC card detection got canceled.");
}

NfcDetection::NfcDetectionState NfcDetection::get_state() const {
    return state;
}

const std::string& NfcDetection::get_last_error() const {
    return lastError;
}

NfcDetection::type_signal_state_changed NfcDetection::signal_state_changed() {
    return m_signal_state_changed;
}

const std::string& NfcDetection::get_card_id() const {
    return cardId;
}

//-----------------------------Events:-----------------------------
void NfcDetection::on_notification_from_worker_thread() {
    // Emit the signal handler:
    m_signal_state_changed.emit(state);
}
}  // namespace backend
