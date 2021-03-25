#pragma once

#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include <jutta_proto/CoffeeMaker.hpp>
#include <gtkmm.h>
#include <memory>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    Gtk::MenuButton* viewMoreBtn{nullptr};
    Gtk::Overlay mainOverlay{};
    Gtk::Box* mainOverlayBox{nullptr};
    widgets::CoffeeMakerDetectionWidget coffeeMakerDetection{};

    std::shared_ptr<jutta_proto::CoffeeMaker> coffeeMaker{nullptr};

 public:
    MainWindow();

 private:
    void prep_window();
    void prep_overview(Gtk::Stack* stack);
    static void prep_advanced(Gtk::Stack* stack);
    void detect_coffee_maker();

    //-----------------------------Events:-----------------------------
    void on_inspector_clicked();
    void on_signal_detection_successfull(std::shared_ptr<jutta_proto::CoffeeMaker> coffeeMaker);
};
}  // namespace ui::windows
