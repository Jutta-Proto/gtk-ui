#include "MainWindow.hpp"
#include "ui/widgets/CoffeeMakerDetectionWidget.hpp"
#include "ui/widgets/NfcCardReaderWidget.hpp"
#include <memory>
#include <gdk/gdkkeysyms.h>
#include <gdkmm/display.h>
#include <giomm/resource.h>
#include <gtk/gtk.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/object.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>
#include <sigc++/functors/mem_fun.h>
#include <spdlog/spdlog.h>

namespace ui::windows {
MainWindow::MainWindow() : nfcCardReader() {
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
    Gtk::Stack* stack = Gtk::make_managed<Gtk::Stack>();
    prep_overview(stack);
    prep_advanced(stack);
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

    // detect_coffee_maker();
    show_nfc_card_detection();
}

void MainWindow::prep_overview(Gtk::Stack* stack) {
    stack->add(mainOverlay, "overview", "Overview");

    Gtk::Box* mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    mainOverlay.add(*mainBox);
    mainBox->set_halign(Gtk::Align::ALIGN_FILL);
    mainBox->set_valign(Gtk::Align::ALIGN_FILL);
    mainBox->set_vexpand(true);
    mainBox->set_homogeneous(false);

    // Predefined coffee:
    coffeeSelectionWidget.set_vexpand(true);
    mainBox->add(coffeeSelectionWidget);
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));
    Glib::RefPtr<Gtk::StyleContext> styleCtx = mainBox->get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("coffee-beans-background");

    Gtk::Box* bottomBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_HORIZONTAL);
    bottomBox->set_homogeneous(true);
    mainBox->add(*bottomBox);

    // Logout:
    Gtk::Button* logoutBtn = Gtk::make_managed<Gtk::Button>("Logout");
    logoutBtn->set_margin_end(10);
    logoutBtn->set_halign(Gtk::Align::ALIGN_END);
    logoutBtn->set_valign(Gtk::Align::ALIGN_CENTER);
    logoutBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_logout_clicked));
    bottomBox->add(*logoutBtn);

    // Custom coffee:
    bottomBox->add(customCoffeeWidget);

    // Placeholder:
    Gtk::Box* placeholderBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    bottomBox->add(*placeholderBox);

    // Overlay:
    mainOverlayBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    mainOverlay.add_overlay(*mainOverlayBox);
    mainOverlayBox->hide();
    Glib::RefPtr<Gtk::StyleContext> overlayStyleCtx = mainOverlayBox->get_style_context();
    overlayStyleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    overlayStyleCtx->add_class("overlay-background");
}

void MainWindow::prep_advanced(Gtk::Stack* stack) {
    Gtk::Box* mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
    stack->add(*mainBox, "advanced", "Advanced");
}

void MainWindow::detect_coffee_maker() {
    if (!coffeeMakerDetectionWidget) {
        coffeeMakerDetectionWidget = Gtk::make_managed<widgets::CoffeeMakerDetectionWidget>();
        coffeeMakerDetectionWidget->signal_detection_successfull().connect(sigc::mem_fun(this, &MainWindow::on_signal_coffee_maker_detection_successfull));
    }
    clear_overlay_children();
    mainOverlayBox->add(*coffeeMakerDetectionWidget);
    mainOverlayBox->show_all();
}

void MainWindow::show_nfc_card_detection() {
    if (!nfcCardDetectionWidget) {
        nfcCardDetectionWidget = Gtk::make_managed<widgets::NfcCardReaderWidget>();
        nfcCardDetectionWidget->signal_detection_canceled().connect(sigc::mem_fun(this, &MainWindow::on_signal_nfc_card_detection_canceled));
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
        SPDLOG_INFO("Default user profile loaded.");
        return;
    }
    SPDLOG_INFO("Loading user profile for '{}'...", cardId);
    SPDLOG_INFO("User profile for '{}' loaded.", cardId);
}

void MainWindow::hide_overlay() {
    mainOverlayBox->hide();
}

//-----------------------------Events:-----------------------------
void MainWindow::on_inspector_clicked() {
    viewMoreBtn->get_popover()->popdown();
    gtk_window_set_interactive_debugging(true);
}

void MainWindow::on_signal_coffee_maker_detection_successfull(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
    coffeeSelectionWidget.set_coffee_maker(this->coffeeMaker);
    customCoffeeWidget.set_coffee_maker(this->coffeeMaker);
    show_nfc_card_detection();
}

void MainWindow::on_signal_nfc_card_detection_canceled() {
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
    show_nfc_card_detection();
}

void MainWindow::on_nfc_card_detected(const std::string& cardId) {
    load_user_profile(cardId);
    hide_overlay();
}
}  // namespace ui::windows
