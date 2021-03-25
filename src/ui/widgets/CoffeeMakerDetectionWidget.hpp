#pragma once

#include "backend/CoffeeMakerDetection.hpp"
#include <optional>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>

namespace ui::widgets {
class CoffeeMakerDetectionWidget : public Gtk::Box {
 private:
    Gtk::Button* actionBtn{nullptr};
    Gtk::Entry* serialPort{nullptr};
    Gtk::Spinner* actionSpinner{nullptr};
    bool detecting{false};

    std::optional<backend::CoffeeMakerDetection> detection{std::nullopt};

 public:
    CoffeeMakerDetectionWidget();

 private:
    void prep_widget();
    void start_detecting();
    void stop_detecting();

    //-----------------------------Events:-----------------------------
    void on_action_btn_click();
};
}  // namespace ui::widgets
