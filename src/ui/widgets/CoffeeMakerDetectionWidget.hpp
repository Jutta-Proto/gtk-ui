#pragma once

#include "backend/CoffeeMakerConnectionHandler.hpp"
#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/infobar.h>

namespace ui::widgets {
class CoffeeMakerDetectionWidget : public Gtk::Frame {
 private:
    Gtk::Button* actionBtn{nullptr};
    Gtk::Entry* btName{nullptr};
    Gtk::Spinner* actionSpinner{nullptr};
    Gtk::InfoBar* errorBar{nullptr};
    Gtk::Box* errorBarBox{nullptr};
    Gtk::Label* errorLabel{nullptr};
    bool detecting{false};

    using type_signal_detection_successfull = sigc::signal<void, std::shared_ptr<jutta_bt_proto::CoffeeMaker>>;
    type_signal_detection_successfull m_signal_detection_successfull;

 public:
    CoffeeMakerDetectionWidget();

    type_signal_detection_successfull signal_detection_successfull();

 private:
    void prep_widget();
    void prep_error_bar();
    void start_detecting();
    static void stop_detecting();

    //-----------------------------Events:-----------------------------
    void on_action_btn_click();
    void on_detection_state_changed(const backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState& state);
    void on_error_bar_response(int response);
};
}  // namespace ui::widgets
