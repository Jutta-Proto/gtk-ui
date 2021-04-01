#pragma once

#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <gtkmm.h>

namespace backend {
class NfcDetection {
 public:
    enum class NfcDetectionState {
        NOT_RUNNING,
        STARTING,
        RUNNING,
        SUCCESS,
        ERROR,
        CANCELD
    };

 private:
    NfcDetectionState state{NfcDetectionState::NOT_RUNNING};
    using type_signal_state_changed = sigc::signal<void, NfcDetectionState>;
    type_signal_state_changed m_signal_state_changed;
    Glib::Dispatcher disp;

    std::optional<std::thread> mainThread;
    std::string lastError{};
    std::string cardId{};

 public:
    NfcDetection();
    ~NfcDetection();

    void start();
    void stop();

    [[nodiscard]] NfcDetectionState get_state() const;
    [[nodiscard]] const std::string& get_last_error() const;
    type_signal_state_changed signal_state_changed();
    [[nodiscard]] const std::string& get_card_id() const;

 private:
    void run();
    void set_state(NfcDetectionState newState);

    //-----------------------------Events:-----------------------------
    void on_notification_from_worker_thread();
};
}  // namespace backend
