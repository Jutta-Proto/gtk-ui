#include "CoffeeMakerDetectionWidget.hpp"
#include "jutta_proto/CoffeeMaker.hpp"
#include <cassert>
#include <cstddef>
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
#include <gtkmm/scrollable.h>
#include <gtkmm/spinner.h>
#include <sigc++/functors/mem_fun.h>

namespace ui::widgets {
CoffeeMakerDetectionWidget::CoffeeMakerDetectionWidget() {
    prep_widget();
}

void CoffeeMakerDetectionWidget::prep_widget() {
    // General:
    set_valign(Gtk::ALIGN_CENTER);
    set_halign(Gtk::ALIGN_CENTER);
    set_vexpand(true);
    set_shadow_type(Gtk::ShadowType::SHADOW_OUT);
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));
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
    Gtk::Label* infoLabel = Gtk::make_managed<Gtk::Label>("Enter the path to the serial port to use:");
    infoLabel->set_line_wrap(true);
    mainBox->add(*infoLabel);

    Gtk::Box* portBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 5);
    portBox->set_margin_top(10);
    actionSpinner = Gtk::make_managed<Gtk::Spinner>();
    portBox->add(*actionSpinner);
    serialPort = Gtk::make_managed<Gtk::Entry>();
    serialPort->set_hexpand(true);
    serialPort->set_placeholder_text("/dev/serial0");
    serialPort->get_buffer()->set_text("/dev/serial0");
    portBox->add(*serialPort);
    mainBox->add(*portBox);

    // Buttons:
    actionBtn = Gtk::make_managed<Gtk::Button>("Detect");
    actionBtn->set_margin_top(10);
    actionBtn->signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerDetectionWidget::on_action_btn_click));
    mainBox->add(*actionBtn);

    show_all();
}

void CoffeeMakerDetectionWidget::start_detecting() {
    prep_error_bar();
    errorBar->hide();

    if (detection) {
        stop_detecting();
    }

    assert(!detection);
    detection = std::make_unique<backend::CoffeeMakerDetection>(std::string(serialPort->get_buffer()->get_text()));
    detection->signal_state_changed().connect(sigc::mem_fun(*this, &CoffeeMakerDetectionWidget::on_detection_state_changed));
    detection->start();
}

void CoffeeMakerDetectionWidget::stop_detecting() {
    assert(detection);
    detection->stop();
    detection->signal_state_changed().clear();
    detection = nullptr;
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

//-----------------------------Events:-----------------------------
void CoffeeMakerDetectionWidget::on_detection_state_changed(const backend::CoffeeMakerDetection::CoffeeMakerDetectionState& state) {
    switch (state) {
        case backend::CoffeeMakerDetection::CoffeeMakerDetectionState::SUCCESS:
            detecting = false;
            actionBtn->set_label("Success!");
            actionSpinner->stop();
            // Emit the signal handler:
            m_signal_detection_successfull.emit(std::make_shared<backend::CoffeeMakerWrapper>(std::string(detection->get_version()), detection->get_connection()));
            break;

        case backend::CoffeeMakerDetection::CoffeeMakerDetectionState::ERROR:
            detecting = false;
            serialPort->set_sensitive(true);
            actionBtn->set_label("Detect");
            actionBtn->set_sensitive(true);
            actionSpinner->stop();

            errorLabel->set_label(detection->get_last_error());
            errorBar->show_all();
            break;

        case backend::CoffeeMakerDetection::CoffeeMakerDetectionState::CANCELD:
            detecting = false;
            serialPort->set_sensitive(false);
            actionBtn->set_label("Canceling...");
            actionBtn->set_sensitive(false);
            actionSpinner->start();
            break;

        case backend::CoffeeMakerDetection::CoffeeMakerDetectionState::RUNNING:
            detecting = true;
            actionBtn->set_label("Cancel");
            actionBtn->set_sensitive(true);
            break;

        case backend::CoffeeMakerDetection::CoffeeMakerDetectionState::NOT_RUNNING:
            detecting = false;
            serialPort->set_sensitive(true);
            actionBtn->set_label("Detect");
            actionBtn->set_sensitive(true);
            actionSpinner->stop();
            break;

        default:
            break;
    }
    serialPort->set_sensitive(true);
}

void CoffeeMakerDetectionWidget::on_error_bar_response(int /*response*/) {
    errorBar->hide();
}

void CoffeeMakerDetectionWidget::on_action_btn_click() {
    if (detecting) {
        detecting = false;
        actionBtn->set_sensitive(false);
        stop_detecting();
    } else {
        detecting = true;
        actionBtn->set_sensitive(true);
        serialPort->set_sensitive(false);
        actionSpinner->start();
        start_detecting();
    }
}
}  // namespace ui::widgets
