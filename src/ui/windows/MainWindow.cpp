#include "MainWindow.hpp"
#include <memory>
#include <gdkmm/display.h>
#include <giomm/resource.h>
#include <gtk/gtk.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/object.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/window.h>

namespace ui::windows {
MainWindow::MainWindow() {
    prep_window();
}

void MainWindow::prep_window() {
    set_title("Jutta Control");
    set_default_size(800, 550);

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

    detect_coffee_maker();
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

    // Custom coffee:
    mainBox->add(customCoffeeWidget);

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
    mainOverlayBox->add(coffeeMakerDetectionWidget);
    mainOverlayBox->show_all();
}

//-----------------------------Events:-----------------------------
void MainWindow::on_inspector_clicked() {
    viewMoreBtn->get_popover()->popdown();
    gtk_window_set_interactive_debugging(true);
}

void MainWindow::on_signal_detection_successfull(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
    coffeeSelectionWidget.set_coffee_maker(this->coffeeMaker);
    customCoffeeWidget.set_coffee_maker(this->coffeeMaker);
    mainOverlayBox->hide();
}
}  // namespace ui::windows
