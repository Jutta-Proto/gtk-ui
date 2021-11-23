#include "CoffeeMakerDetectionWidget.hpp"
#include "backend/CoffeeMakerConnectionHandler.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>
#include <cstddef>
#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <optional>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/enums.h>
#include <gtkmm/frame.h>
#include <gtkmm/infobar.h>
#include <gtkmm/label.h>
#include <gtkmm/paned.h>
#include <gtkmm/spinner.h>
#include <sigc++/functors/mem_fun.h>

namespace ui::widgets {
CoffeeMakerDetectionWidget::CoffeeMakerDetectionWidget() {
    prep_widget();
    backend::CoffeeMakerConnectionHandler::get_instance().signal_state_changed().connect(sigc::mem_fun(this, &CoffeeMakerDetectionWidget::on_detection_state_changed));
}

void CoffeeMakerDetectionWidget::prep_widget() {
    // General:
    set_valign(Gtk::ALIGN_CENTER);
    set_halign(Gtk::ALIGN_CENTER);
    set_vexpand(true);
    set_shadow_type(Gtk::ShadowType::SHADOW_OUT);
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("overlay-frame-background");

    Gtk::Box* mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
    mainBox->set_margin_bottom(10);
    mainBox->set_margin_start(10);
    mainBox->set_margin_top(10);
    mainBox->set_margin_end(10);
    add(*mainBox);

    // Error:
    errorBarBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
    mainBox->add(*errorBarBox);

    // Content:
    Gtk::Label* infoLabel = Gtk::make_managed<Gtk::Label>("Enter BT name of the coffee maker:");
    infoLabel->set_line_wrap(true);
    mainBox->add(*infoLabel);

    Gtk::Box* portBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 5);
    portBox->set_margin_top(10);
    actionSpinner = Gtk::make_managed<Gtk::Spinner>();
    portBox->add(*actionSpinner);
    btName = Gtk::make_managed<Gtk::Entry>();
    btName->set_hexpand(true);
    btName->set_placeholder_text("TT214H BlueFrog");
    btName->get_buffer()->set_text("TT214H BlueFrog");
    portBox->add(*btName);
    mainBox->add(*portBox);

    // Buttons:
    actionBtn = Gtk::make_managed<Gtk::Button>("Connect");
    actionBtn->set_margin_top(10);
    actionBtn->signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerDetectionWidget::on_action_btn_click));
    mainBox->add(*actionBtn);

    show_all();
}

void CoffeeMakerDetectionWidget::start_detecting() {
    prep_error_bar();
    errorBar->hide();

    backend::CoffeeMakerConnectionHandler::get_instance().set_name(std::string(btName->get_buffer()->get_text()));
    if (backend::CoffeeMakerConnectionHandler::get_instance().get_state() == backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::NOT_RUNNING) {
        backend::CoffeeMakerConnectionHandler::get_instance().connect();
    }
}

void CoffeeMakerDetectionWidget::stop_detecting() {
    backend::CoffeeMakerConnectionHandler::get_instance().disconnect();
}

void CoffeeMakerDetectionWidget::prep_error_bar() {
    if (errorBar) {
        return;
    }

    errorBar = Gtk::make_managed<Gtk::InfoBar>();
    errorBar->set_message_type(Gtk::MessageType::MESSAGE_ERROR);
    errorBar->set_show_close_button(true);
    errorBar->set_margin_bottom(10);
    errorLabel = Gtk::make_managed<Gtk::Label>("");
    errorLabel->set_line_wrap(true);
    errorLabel->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
    static_cast<Gtk::Box*>(errorBar->get_content_area())->add(*errorLabel);
    errorBar->signal_response().connect(sigc::mem_fun(this, &CoffeeMakerDetectionWidget::on_error_bar_response));
    errorBarBox->add(*errorBar);
}

CoffeeMakerDetectionWidget::type_signal_detection_successfull CoffeeMakerDetectionWidget::signal_detection_successfull() {
    return m_signal_detection_successfull;
}

//-----------------------------Events:-----------------------------
void CoffeeMakerDetectionWidget::on_detection_state_changed(const backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState& state) {
    switch (state) {
        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::CONNECTED:
            detecting = false;
            actionBtn->set_label("Success!");
            actionSpinner->stop();
            // Emit the signal handler:
            m_signal_detection_successfull.emit(backend::CoffeeMakerConnectionHandler::get_instance().get_coffee_maker());
            break;

        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::NOT_RUNNING:
            detecting = false;
            btName->set_sensitive(true);
            actionBtn->set_label("Connect");
            actionBtn->set_sensitive(true);
            actionSpinner->stop();
            break;

        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::SEARCHING:
        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::CONNECTING:
            detecting = true;
            actionBtn->set_label("Cancel");
            actionBtn->set_sensitive(true);
            break;

        default:
            break;
    }
    btName->set_sensitive(true);
}

void CoffeeMakerDetectionWidget::on_error_bar_response(int /*response*/) {
    errorBar->hide();
}

void CoffeeMakerDetectionWidget::on_action_btn_click() {
    if (detecting) {
        actionBtn->set_sensitive(false);
        stop_detecting();
    } else {
        actionBtn->set_sensitive(true);
        btName->set_sensitive(false);
        actionSpinner->start();
        start_detecting();
    }
    detecting = !detecting;
}
}  // namespace ui::widgets
