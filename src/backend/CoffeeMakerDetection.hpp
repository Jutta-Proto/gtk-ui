#pragma once

#include <jutta_proto/JuttaConnection.hpp>
#include <optional>
#include <string>
#include <thread>
#include <gtkmm.h>
#include <memory>

namespace backend {
class CoffeeMakerDetection {
 public:
    enum class CoffeeMakerDetectionState {
        NOT_RUNNING,
        STARTING,
        RUNNING,
        SUCCESS,
        ERROR,
        CANCELD
    };

 private:
    CoffeeMakerDetectionState state{CoffeeMakerDetectionState::NOT_RUNNING};
    using type_signal_state_changed = sigc::signal<void, CoffeeMakerDetectionState>;
    type_signal_state_changed m_signal_state_changed;
    Glib::Dispatcher disp;

    std::optional<std::thread> mainThread;
    std::string lastError{};
    std::unique_ptr<jutta_proto::JuttaConnection> connection;
    std::string version{};

 public:
    explicit CoffeeMakerDetection(std::string&& device);
    ~CoffeeMakerDetection();

    void start();
    void stop();

    [[nodiscard]] CoffeeMakerDetectionState get_state() const;
    std::unique_ptr<jutta_proto::JuttaConnection>&& get_connection();
    [[nodiscard]] const std::string& get_last_error() const;
    [[nodiscard]] const std::string& get_version() const;
    type_signal_state_changed signal_state_changed();

 private:
    void run();
    void set_state(CoffeeMakerDetectionState newState);

    //-----------------------------Events:-----------------------------
    void on_notification_from_worker_thread();
};
}  // namespace backend
