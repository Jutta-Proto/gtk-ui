#include "DetectCoffeeMakerDialog.hpp"
#include <optional>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/spinner.h>

namespace ui::dialogs {
DetectCoffeeMakerDialog::DetectCoffeeMakerDialog() : Gtk::Dialog("Detect Coffee Maker", true) {
    prep_dialog();
}

void DetectCoffeeMakerDialog::prep_dialog() {
    Gtk::Label* infoLabel = Gtk::make_managed<Gtk::Label>("Enter the path to the serial port to use:");
    infoLabel->set_line_wrap(true);
    get_content_area()->add(*infoLabel);

    Gtk::Box* portBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
    actionSpinner = Gtk::make_managed<Gtk::Spinner>();
    actionSpinner->set_margin_right(10);
    portBox->add(*actionSpinner);
    serialPort = Gtk::make_managed<Gtk::Entry>();
    serialPort->set_placeholder_text("/dev/serial0");
    serialPort->get_buffer()->set_text("/dev/serial0");
    portBox->add(*serialPort);
    get_content_area()->add(*portBox);

    // Buttons:
    actionBtn = Gtk::make_managed<Gtk::Button>("Detect");
    actionBtn->set_margin_top(10);
    actionBtn->signal_clicked().connect(sigc::mem_fun(*this, &DetectCoffeeMakerDialog::on_action_btn_click));
    get_content_area()->add(*actionBtn);

    show_all();
}

void DetectCoffeeMakerDialog::on_action_btn_click() {
    if (detecting) {
        detecting = false;
        actionBtn->set_label("Detect");
        serialPort->set_sensitive(true);
        actionSpinner->stop();
        stop_detecting();
        set_title("Detect Coffee Maker");
    } else {
        detecting = true;
        actionBtn->set_label("Cancel");
        serialPort->set_sensitive(false);
        actionSpinner->start();
        start_detecting();
        set_title("Detecting Coffee Maker...");
    }
}

void DetectCoffeeMakerDialog::start_detecting() {
    detection = std::make_optional<backend::CoffeeMakerDetection>(std::string(serialPort->get_buffer()->get_text()));
    detection->start();
}

void DetectCoffeeMakerDialog::stop_detecting() {
    detection->stop();
}
}  // namespace ui::dialogs
