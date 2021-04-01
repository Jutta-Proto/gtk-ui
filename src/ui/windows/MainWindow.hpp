#pragma once

#include "backend/CoffeeMakerWrapper.hpp"
#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include "ui/widgets/CoffeeSelectionWidget.hpp"
#include "ui/widgets/CustomCoffeeWidget.hpp"
#include "ui/widgets/NfcCardReaderWidget.hpp"
#include <jutta_proto/CoffeeMaker.hpp>
#include <memory>
#include <gtkmm.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    bool inFullScreen{false};
    Gtk::MenuButton* viewMoreBtn{nullptr};
    Gtk::Overlay mainOverlay{};
    Gtk::Box* mainOverlayBox{nullptr};
    widgets::CoffeeMakerDetectionWidget* coffeeMakerDetectionWidget{nullptr};
    widgets::NfcCardReaderWidget* nfcCardDetectionWidget{nullptr};
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
    void detect_nfc_card();
    void hide_overlay();
    void clear_overlay_children();
    static void load_user_profile(const std::string& cardId);

    //-----------------------------Events:-----------------------------
    void on_inspector_clicked();
    void on_signal_coffee_maker_detection_successfull(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker);
    void on_signal_nfc_card_detection_successfull(const std::string& cardId);
    void on_signal_nfc_card_detection_canceled();
    void on_full_screen_clicked();
    void on_logout_clicked();
    bool on_key_pressed(GdkEventKey* event);
    bool on_window_state_changed(GdkEventWindowState* state);
};
}  // namespace ui::windows
