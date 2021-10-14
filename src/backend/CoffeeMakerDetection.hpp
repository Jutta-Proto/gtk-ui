#pragma once

#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <gtkmm.h>

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
    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker;
    std::string btName;
    bool canceled{true};

 public:
    explicit CoffeeMakerDetection(std::string&& btName);
    ~CoffeeMakerDetection();

    void start();
    void stop();

    [[nodiscard]] CoffeeMakerDetectionState get_state() const;
    std::shared_ptr<jutta_bt_proto::CoffeeMaker>&& get_coffee_maker();
    [[nodiscard]] const std::string& get_last_error() const;
    type_signal_state_changed signal_state_changed();

 private:
    void run();
    void set_state(CoffeeMakerDetectionState newState);

    static bool starts_with(const std::string& s, const std::string& prefix);
    static bool ends_with(const std::string& s, const std::string& postfix);
    void try_join();

    //-----------------------------Events:-----------------------------
    void on_notification_from_worker_thread();
};
}  // namespace backend
