#pragma once

#include "backend/NfcCardReader.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include "ui/widgets/CoffeeSelectionWidget.hpp"
#include "ui/widgets/EditCustomCoffeeWidget.hpp"
#include "ui/widgets/NfcCardReaderWidget.hpp"
#include "ui/widgets/StatusBarWidget.hpp"
#include "ui/widgets/StatusOverlayWidget.hpp"
#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <optional>
#include <gtkmm.h>
#include <gtkmm/label.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    bool inFullScreen{false};
    Gtk::Stack* stack{nullptr};
    Gtk::MenuButton* viewMoreBtn{nullptr};
    Gtk::Overlay mainOverlay{};
    Gtk::Box* mainOverlayBox{nullptr};
    widgets::CoffeeMakerDetectionWidget* coffeeMakerDetectionWidget{nullptr};
    widgets::NfcCardReaderWidget* nfcCardDetectionWidget{nullptr};
    widgets::EditCustomCoffeeWidget editCustomCoffeeWidget{};
    widgets::CoffeeSelectionWidget coffeeSelectionWidget{};
    Gtk::Overlay statusOverlay{};
    widgets::StatusOverlayWidget statusOverlayWidget{};
    Gtk::Box* statusOverlayBox{nullptr};
    widgets::StatusBarWidget statusBarWidget{};

    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker{nullptr};
    Glib::Dispatcher alertsChangedDisp;
    std::optional<eventpp::CallbackList<void(const std::vector<const jutta_bt_proto::Alert*>&)>::Handle> alertsHandle;
    backend::NfcCardReader nfcCardReader{};
    bool skipNextLogoutClicked{false};

 public:
    MainWindow();
    MainWindow(MainWindow&& old) = delete;
    MainWindow(const MainWindow& other) = delete;
    ~MainWindow() override;

    MainWindow& operator=(MainWindow&& old) = delete;
    MainWindow& operator=(const MainWindow& other) = delete;

 private:
    void prep_window();
    void prep_overview_stack_page(Gtk::Stack* stack);
    void prep_custom_coffee_stack_page(Gtk::Stack* stack);
    static void prep_advanced_stack_page(Gtk::Stack* stack);
    void show_detect_coffee_maker();
    void show_nfc_card_detection();
    void show_status_overlay();
    void hide_status_overlay();
    void hide_main_overlay();
    void clear_main_overlay_children();

    void load_user_profile(const std::string& cardId);
    void load_user_profile(backend::storage::UserProfile* profile);
    void load_user(std::string userId);

    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);

    //-----------------------------Events:-----------------------------
    void on_inspector_clicked();
    void on_coffee_maker_detection_successfull(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);
    void on_nfc_card_detection_canceled();
    void on_nfc_card_detected(const std::string& cardId);
    void on_full_screen_clicked();
    void on_edit_custom_coffee_clicked();
    void on_logout_clicked();
    void on_disconnect_clicked();
    bool on_key_pressed(GdkEventKey* event);
    bool on_window_state_changed(GdkEventWindowState* state);
    void on_custom_coffee_back_clicked();
    void on_custom_coffee_profile_value_changed(backend::storage::UserProfile* profile);
    void on_alerts_changed();
};
}  // namespace ui::windows
