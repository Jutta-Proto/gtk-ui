#pragma once

#include "FixedSizeStack.hpp"
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <gtkmm.h>

namespace backend {
class NfcCardReader {
 public:
    enum class NfcCardReaderState {
        NOT_RUNNING,
        STARTING,
        RUNNING,
        STOPPING
    };

 private:
    NfcCardReaderState state{NfcCardReaderState::NOT_RUNNING};
    using type_signal_state_changed = sigc::signal<void, NfcCardReaderState>;
    using type_signal_card_detected = sigc::signal<void, const std::string&>;
    type_signal_state_changed m_signal_state_changed;
    type_signal_card_detected m_signal_card_detected;
    Glib::Dispatcher stateChangedDisp;
    Glib::Dispatcher cardDetectedDisp;

    std::optional<std::thread> mainThread;
    FixedSizeStack stack{};
    std::mutex stackMutex{};
    std::string lastCardId{};
    std::chrono::steady_clock::time_point lastChar = std::chrono::steady_clock::time_point::min();

 public:
    NfcCardReader();
    ~NfcCardReader();

    void start();
    void stop();

    void on_new_char_input(char c);

    [[nodiscard]] NfcCardReaderState get_state() const;
    type_signal_state_changed signal_state_changed();
    type_signal_card_detected signal_card_detected();

 private:
    void run();
    void set_state(NfcCardReaderState newState);

    //-----------------------------Events:-----------------------------
    void on_state_changed_notification_from_worker_thread();
    void on_card_detected_notification_from_worker_thread();
};
}  // namespace backend
