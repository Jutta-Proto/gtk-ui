#include "NfcCardReaderWidget.hpp"
#include "backend/NfcDetection.hpp"
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
#include <gtkmm/image.h>
#include <gtkmm/infobar.h>
#include <gtkmm/label.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinner.h>
#include <sigc++/functors/mem_fun.h>

namespace ui::widgets {
NfcCardReaderWidget::NfcCardReaderWidget() {
    prep_widget();
}

void NfcCardReaderWidget::prep_widget() {
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

    // Info:
    Gtk::Label* infoLabel = Gtk::make_managed<Gtk::Label>("Place your card on the scanner or continue without a card.");
    infoLabel->set_line_wrap(true);
    mainBox->add(*infoLabel);

    // Content:
    Gtk::Box* contentBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_HORIZONTAL, 0);
    contentBox->set_margin_top(10);
    contentBox->set_halign(Gtk::Align::ALIGN_CENTER);
    mainBox->add(*contentBox);

    // NFC:
    Gtk::Box* nfcBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
    nfcBox->set_valign(Gtk::Align::ALIGN_CENTER);
    contentBox->add(*nfcBox);

    nfcImageBuf = Gdk::Pixbuf::create_from_resource("/ui/nfc.png", 128, 128, true);
    Gtk::Image* nfcImage = Gtk::make_managed<Gtk::Image>(nfcImageBuf);
    contentBox->add(*nfcImage);

    runningSpinner = Gtk::make_managed<Gtk::Spinner>();
    nfcBox->add(*runningSpinner);

    // Seperator:
    Gtk::Separator* seperator = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::ORIENTATION_VERTICAL);
    seperator->set_margin_bottom(10);
    seperator->set_margin_start(10);
    seperator->set_margin_top(10);
    seperator->set_margin_end(10);
    seperator->set_valign(Gtk::Align::ALIGN_FILL);
    contentBox->add(*seperator);

    // Cancel Button:
    cancelBtn = Gtk::make_managed<Gtk::Button>("Continue");
    cancelBtn->set_tooltip_text("Continue without an NFC card.");
    cancelBtn->set_valign(Gtk::Align::ALIGN_CENTER);
    cancelBtn->signal_clicked().connect(sigc::mem_fun(*this, &NfcCardReaderWidget::on_cancel_btn_click));
    contentBox->add(*cancelBtn);

    show_all();
}

void NfcCardReaderWidget::start_detecting() {
    prep_error_bar();
    errorBar->hide();

    if (detection) {
        stop_detecting();
    }

    assert(!detection);
    detection = std::make_unique<backend::NfcDetection>();
    detection->signal_state_changed().connect(sigc::mem_fun(*this, &NfcCardReaderWidget::on_detection_state_changed));
    detection->start();
}

void NfcCardReaderWidget::stop_detecting() {
    assert(detection);
    detection->stop();
    detection->signal_state_changed().clear();
    detection = nullptr;
}

void NfcCardReaderWidget::prep_error_bar() {
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
    errorBar->signal_response().connect(sigc::mem_fun(this, &NfcCardReaderWidget::on_error_bar_response));
    errorBarBox->add(*errorBar);
}

NfcCardReaderWidget::type_signal_detection_successfull NfcCardReaderWidget::signal_detection_successfull() {
    return m_signal_detection_successfull;
}

NfcCardReaderWidget::type_signal_detection_canceled NfcCardReaderWidget::signal_detection_canceled() {
    return m_signal_detection_canceled;
}

//-----------------------------Events:-----------------------------
void NfcCardReaderWidget::on_detection_state_changed(const backend::NfcDetection::NfcDetectionState& state) {
    switch (state) {
        case backend::NfcDetection::NfcDetectionState::SUCCESS:
            runningSpinner->stop();
            // Emit the signal handler:
            m_signal_detection_successfull.emit(detection->get_card_id());
            break;

        case backend::NfcDetection::NfcDetectionState::ERROR:
            runningSpinner->stop();

            errorLabel->set_label(detection->get_last_error());
            errorBar->show_all();
            break;

        case backend::NfcDetection::NfcDetectionState::RUNNING:
            runningSpinner->start();
            break;

        case backend::NfcDetection::NfcDetectionState::CANCELD:
        case backend::NfcDetection::NfcDetectionState::NOT_RUNNING:
        default:
            runningSpinner->stop();
            break;
    }
}

void NfcCardReaderWidget::on_error_bar_response(int /*response*/) {
    errorBar->hide();
}

void NfcCardReaderWidget::on_cancel_btn_click() {
    m_signal_detection_canceled.emit();
}
}  // namespace ui::widgets
