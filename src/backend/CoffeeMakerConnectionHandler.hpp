#pragma once

#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <gtkmm.h>

namespace backend {
class CoffeeMakerConnectionHandler {
 public:
    enum class CoffeeMakerConnectionHandlerState {
        NOT_RUNNING,
        SEARCHING,
        CONNECTING,
        CONNECTED
    };

 private:
    CoffeeMakerConnectionHandlerState state{CoffeeMakerConnectionHandlerState::NOT_RUNNING};
    using type_signal_state_changed = sigc::signal<void, CoffeeMakerConnectionHandlerState>;
    type_signal_state_changed m_signal_state_changed;
    Glib::Dispatcher disp;

    std::optional<std::thread> mainThread;
    std::string lastError{};
    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker;
    std::string btName;
    bool canceled{true};

    CoffeeMakerConnectionHandler();
    ~CoffeeMakerConnectionHandler();

 public:
    void set_name(std::string&& btName);
    void connect();
    void disconnect();

    [[nodiscard]] CoffeeMakerConnectionHandlerState get_state() const;
    std::shared_ptr<jutta_bt_proto::CoffeeMaker>&& get_coffee_maker();
    [[nodiscard]] const std::string& get_last_error() const;
    type_signal_state_changed signal_state_changed();

    static CoffeeMakerConnectionHandler& get_instance();

 private:
    void start();
    void stop();
    void run();
    void set_state(CoffeeMakerConnectionHandlerState newState);

    static bool starts_with(const std::string& s, const std::string& prefix);
    static bool ends_with(const std::string& s, const std::string& postfix);
    void try_join();

    //-----------------------------Events:-----------------------------
    void on_notification_from_worker_thread();
    void on_coffee_maker_state_changed(const jutta_bt_proto::CoffeeMakerState& state);
};
}  // namespace backend
