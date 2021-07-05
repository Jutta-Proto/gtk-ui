#include "MainWindow.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include "ui/utils/UiUtils.hpp"
#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include "ui/widgets/NfcCardReaderWidget.hpp"
#include <memory>
#include <gdk/gdkkeysyms.h>
#include <gdkmm/display.h>
#include <giomm/resource.h>
#include <gtk/gtk.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>
#include <sigc++/functors/mem_fun.h>
#include <spdlog/spdlog.h>

namespace ui::windows {
MainWindow::MainWindow() {
    prep_window();
    nfcCardReader.signal_card_detected().connect(sigc::mem_fun(this, &MainWindow::on_nfc_card_detected));
    nfcCardReader.start();
}

MainWindow::~MainWindow() {
    nfcCardReader.stop();
}

void MainWindow::prep_window() {
    set_title("Jutta Control");
    set_default_size(800, 550);
    signal_key_press_event().connect(sigc::mem_fun(this, &MainWindow::on_key_pressed));
    signal_window_state_event().connect(sigc::mem_fun(this, &MainWindow::on_window_state_changed));
    fullscreen();

    // Content:
    stack = Gtk::make_managed<Gtk::Stack>();
    prep_overview_stack_page(stack);
    prep_custom_coffee_stack_page(stack);
    prep_advanced_stack_page(stack);
    add(*stack);

    // Header bar:
    Gtk::HeaderBar* headerBar = Gtk::make_managed<Gtk::HeaderBar>();
    viewMoreBtn = Gtk::make_managed<Gtk::MenuButton>();
    viewMoreBtn->set_image_from_icon_name("open-menu");
    Gtk::PopoverMenu* viewMorePopover = Gtk::make_managed<Gtk::PopoverMenu>();
    Gtk::Stack* viewMoreMenuStack = Gtk::make_managed<Gtk::Stack>();
    Gtk::Box* viewMoreMenuBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    Gtk::Button* fullscreenBtn = Gtk::make_managed<Gtk::Button>("Fullscreen");
    fullscreenBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_full_screen_clicked));
    viewMoreMenuBox->add(*fullscreenBtn);
    Gtk::Button* settingsBtn = Gtk::make_managed<Gtk::Button>("Settings");
    viewMoreMenuBox->add(*settingsBtn);
    Gtk::Button* inspectorBtn = Gtk::make_managed<Gtk::Button>("Inspector");
    inspectorBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_inspector_clicked));
    viewMoreMenuBox->add(*inspectorBtn);
    Gtk::Button* aboutBtn = Gtk::make_managed<Gtk::Button>("About");
    viewMoreMenuBox->add(*aboutBtn);
    viewMoreMenuStack->add(*viewMoreMenuBox, "main");
    viewMoreMenuStack->show_all();
    viewMorePopover->add(*viewMoreMenuStack);
    viewMoreBtn->set_popover(*viewMorePopover);
    headerBar->pack_end(*viewMoreBtn);

    Gtk::StackSwitcher* stackSwitcher = Gtk::make_managed<Gtk::StackSwitcher>();
    stackSwitcher->set_stack(*stack);
    headerBar->set_custom_title(*stackSwitcher);
    set_titlebar(*headerBar);

    show_all();

    show_detect_coffee_maker();
    // show_nfc_card_detection();
}

void MainWindow::prep_overview_stack_page(Gtk::Stack* stack) {
    stack->add(mainOverlay, "overview", "Overview");

    Gtk::Box* mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    mainOverlay.add(*mainBox);
    mainBox->set_halign(Gtk::Align::ALIGN_FILL);
    mainBox->set_valign(Gtk::Align::ALIGN_FILL);
    mainBox->set_vexpand(true);
    mainBox->set_homogeneous(false);

    // Predefined coffee:
    Gtk::ScrolledWindow* coffeeSelectionWindow = Gtk::make_managed<Gtk::ScrolledWindow>();
    coffeeSelectionWidget.signal_edit_custom_coffee_clicked().connect(sigc::mem_fun(this, &MainWindow::on_edit_custom_coffee_clicked));
    coffeeSelectionWindow->add(coffeeSelectionWidget);
    coffeeSelectionWindow->set_policy(Gtk::PolicyType::POLICY_NEVER, Gtk::PolicyType::POLICY_AUTOMATIC);
    coffeeSelectionWindow->set_vexpand(true);
    mainBox->add(*coffeeSelectionWindow);
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = mainBox->get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("coffee-beans-background");

    // Status bar:
    statusBarWidget.signal_logout_clicked().connect(sigc::mem_fun(this, &MainWindow::on_logout_clicked));
    statusBarWidget.signal_reconnect_clicked().connect(sigc::mem_fun(this, &MainWindow::on_reconnect_clicked));
    mainBox->add(statusBarWidget);

    // Overlay:
    mainOverlayBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    mainOverlay.add_overlay(*mainOverlayBox);
    mainOverlayBox->hide();
    Glib::RefPtr<Gtk::StyleContext> overlayStyleCtx = mainOverlayBox->get_style_context();
    overlayStyleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    overlayStyleCtx->add_class("overlay-background");
}

void MainWindow::prep_advanced_stack_page(Gtk::Stack* stack) {
    Gtk::Box* mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
    stack->add(*mainBox, "advanced", "Advanced");
}

void MainWindow::prep_custom_coffee_stack_page(Gtk::Stack* stack) {
    stack->add(editCustomCoffeeWidget, "custom_coffee", "Custom Coffee");
    editCustomCoffeeWidget.signal_profile_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_custom_coffee_profile_value_changed));
    editCustomCoffeeWidget.signal_back_clicked().connect(sigc::mem_fun(this, &MainWindow::on_custom_coffee_back_clicked));
    stack->set_visible_child(editCustomCoffeeWidget);
}

void MainWindow::show_detect_coffee_maker() {
    if (!coffeeMakerDetectionWidget) {
        coffeeMakerDetectionWidget = Gtk::make_managed<widgets::CoffeeMakerDetectionWidget>();
        coffeeMakerDetectionWidget->signal_detection_successfull().connect(sigc::mem_fun(this, &MainWindow::on_coffee_maker_detection_successfull));
    }
    clear_overlay_children();
    mainOverlayBox->add(*coffeeMakerDetectionWidget);
    mainOverlayBox->show_all();
}

void MainWindow::show_nfc_card_detection() {
    if (!nfcCardDetectionWidget) {
        nfcCardDetectionWidget = Gtk::make_managed<widgets::NfcCardReaderWidget>();
        nfcCardDetectionWidget->signal_detection_canceled().connect(sigc::mem_fun(this, &MainWindow::on_nfc_card_detection_canceled));
    }
    clear_overlay_children();
    mainOverlayBox->add(*nfcCardDetectionWidget);
    mainOverlayBox->show_all();
}

void MainWindow::clear_overlay_children() {
    for (Gtk::Widget* widget : mainOverlayBox->get_children()) {
        mainOverlayBox->remove(*widget);
    }
}

void MainWindow::load_user_profile(const std::string& cardId) {
    if (cardId.empty()) {
        SPDLOG_INFO("Loading default user profile...");
        load_user_profile(backend::storage::UserProfileStorage::get_default_profile());
        SPDLOG_INFO("Default user profile loaded.");
        return;
    }
    SPDLOG_INFO("Loading user profile for '{}'...", cardId);
    load_user_profile(backend::storage::get_user_profile_storage_instance().get_profile(std::string(cardId)));
    SPDLOG_INFO("User profile for '{}' loaded.", cardId);
}

void MainWindow::load_user_profile(backend::storage::UserProfile* profile) {
    editCustomCoffeeWidget.set_user_profile(profile);
    coffeeSelectionWidget.set_user_profile(profile);
    statusBarWidget.set_user_profile(profile);
}

void MainWindow::hide_overlay() {
    mainOverlayBox->hide();
}

//-----------------------------Events:-----------------------------
void MainWindow::on_inspector_clicked() {
    viewMoreBtn->get_popover()->popdown();
    gtk_window_set_interactive_debugging(true);
}

void MainWindow::on_coffee_maker_detection_successfull(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
    coffeeSelectionWidget.set_coffee_maker(this->coffeeMaker);
    show_nfc_card_detection();
}

void MainWindow::on_nfc_card_detection_canceled() {
    load_user_profile("");
    hide_overlay();
}

void MainWindow::on_full_screen_clicked() {
    fullscreen();
}

bool MainWindow::on_key_pressed(GdkEventKey* event) {
    if (event->keyval == GDK_KEY_Escape && inFullScreen) {
        unfullscreen();
        maximize();
        return true;
    }
    if (event->keyval == GDK_KEY_F11) {
        if (inFullScreen) {
            unfullscreen();
            maximize();
        } else {
            fullscreen();
        }
        return true;
    }
    if (event->length == 1) {
        nfcCardReader.on_new_char_input(*(event->string));
    }
    return false;
}

bool MainWindow::on_window_state_changed(GdkEventWindowState* state) {
    inFullScreen = state->new_window_state & GDK_WINDOW_STATE_FULLSCREEN;
    return false;
}

void MainWindow::on_logout_clicked() {
    if (skipNextLogoutClicked) {
        skipNextLogoutClicked = false;
        return;
    }
    load_user_profile(nullptr);
    show_nfc_card_detection();
}

void MainWindow::on_reconnect_clicked() {
    show_detect_coffee_maker();
}

void MainWindow::on_nfc_card_detected(const std::string& cardId) {
    load_user_profile(cardId);
    hide_overlay();
    // Once a card has been detected, enter is being pressed. Skip this false activation:
    skipNextLogoutClicked = true;
}

void MainWindow::on_edit_custom_coffee_clicked() {
    stack->set_visible_child(editCustomCoffeeWidget);
}

void MainWindow::on_custom_coffee_back_clicked() {
    stack->set_visible_child(mainOverlay);
}

void MainWindow::on_custom_coffee_profile_value_changed(backend::storage::UserProfile* /*profile*/) {
}
}  // namespace ui::windows
