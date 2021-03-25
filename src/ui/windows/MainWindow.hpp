#pragma once

#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include <gtkmm.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    Gtk::MenuButton* viewMoreBtn{nullptr};
    Gtk::Overlay mainOverlay{};
    Gtk::Box* mainOverlayBox{nullptr};
    widgets::CoffeeMakerDetectionWidget coffeeMakerDetection{};

 public:
    MainWindow();

 private:
    void prep_window();
    void prep_overview(Gtk::Stack* stack);
    static void prep_advanced(Gtk::Stack* stack);
    void detect_coffee_maker();

    //-----------------------------Events:-----------------------------
    void on_inspector_clicked();
};
}  // namespace ui::windows
