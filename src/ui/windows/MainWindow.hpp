#pragma once

#include "backend/CoffeeMakerWrapper.hpp"
#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include "ui/widgets/CoffeeSelectionWidget.hpp"
#include "ui/widgets/CustomCoffeeWidget.hpp"
#include <jutta_proto/CoffeeMaker.hpp>
#include <memory>
#include <gtkmm.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    Gtk::MenuButton* viewMoreBtn{nullptr};
    Gtk::Overlay mainOverlay{};
    Gtk::Box* mainOverlayBox{nullptr};
    widgets::CoffeeMakerDetectionWidget coffeeMakerDetectionWidget{};
    widgets::CustomCoffeeWidget customCoffeeWidget{};
    widgets::CoffeeSelectionWidget coffeeSelectionWidget{};

    std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker{nullptr};

 public:
    MainWindow();

 private:
    void prep_window();
    void prep_overview(Gtk::Stack* stack);
    static void prep_advanced(Gtk::Stack* stack);
    void detect_coffee_maker();

    //-----------------------------Events:-----------------------------
    void on_inspector_clicked();
    void on_signal_detection_successfull(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker);
};
}  // namespace ui::windows
