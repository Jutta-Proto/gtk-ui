#pragma once

#include "backend/CoffeeMakerDetection.hpp"
#include <optional>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>

namespace ui::dialogs {
class DetectCoffeeMakerDialog : public Gtk::Dialog {
 private:
    Gtk::Button* actionBtn{nullptr};
    Gtk::Entry* serialPort{nullptr};
    Gtk::Spinner* actionSpinner{nullptr};
    bool detecting{false};

    std::optional<backend::CoffeeMakerDetection> detection{std::nullopt};

 public:
    DetectCoffeeMakerDialog();

 private:
    void prep_dialog();
    void start_detecting();
    void stop_detecting();

    //-----------------------------Events:-----------------------------
    void on_action_btn_click();
};
}  // namespace ui::dialogs
