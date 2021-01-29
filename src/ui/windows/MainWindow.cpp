#include "MainWindow.hpp"
#include "ui/widgets/CoffeeSelection.hpp"
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
    set_child(*stack);

    // Header bar:
    Gtk::HeaderBar* headerBar = Gtk::make_managed<Gtk::HeaderBar>();
    viewMoreBtn = Gtk::make_managed<Gtk::MenuButton>();
    viewMoreBtn->set_icon_name("open-menu");
    Gtk::PopoverMenu* viewMorePopover = Gtk::make_managed<Gtk::PopoverMenu>();
    Gtk::Stack* viewMoreMenuStack = Gtk::make_managed<Gtk::Stack>();
    Gtk::Box* viewMoreMenuBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    Gtk::Button* settingsBtn = Gtk::make_managed<Gtk::Button>("Settings");
    viewMoreMenuBox->append(*settingsBtn);
    Gtk::Button* inspectorBtn = Gtk::make_managed<Gtk::Button>("Inspector");
    inspectorBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_inspector_clicked));
    viewMoreMenuBox->append(*inspectorBtn);
    Gtk::Button* aboutBtn = Gtk::make_managed<Gtk::Button>("About");
    viewMoreMenuBox->append(*aboutBtn);
    viewMoreMenuStack->add(*viewMoreMenuBox, "main");
    viewMorePopover->set_child(*viewMoreMenuStack);
    viewMoreBtn->set_popover(*viewMorePopover);
    headerBar->pack_end(*viewMoreBtn);

    Gtk::StackSwitcher* stackSwitcher = Gtk::make_managed<Gtk::StackSwitcher>();
    stackSwitcher->set_stack(*stack);
    // stackSwitcher->get_first_child()->get_first_child()->add_css_class("suggested-action");
    headerBar->set_title_widget(*stackSwitcher);

    set_titlebar(*headerBar);

    // CSS Style:
    /*Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_path("theme.css"));
    get_style_context()->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);*/
}

void MainWindow::prep_overview(Gtk::Stack* stack) {
    Gtk::Box* box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    box->set_halign(Gtk::Align::FILL);
    box->set_valign(Gtk::Align::FILL);
    box->set_vexpand(true);
    box->set_homogeneous();
    widgets::CoffeeSelection* coffeeSelection = Gtk::make_managed<widgets::CoffeeSelection>();
    box->append(*coffeeSelection);
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));
    box->get_style_context()->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    box->add_css_class("coffee-beans-background");
    /*Gtk::Frame* frame = Gtk::make_managed<Gtk::Frame>();
    widgets::CoffeeSelection* coffeeSelection = Gtk::make_managed<widgets::CoffeeSelection>();
    frame->set_child(*coffeeSelection);
    frame->set_halign(Gtk::Align::CENTER);
    frame->set_valign(Gtk::Align::CENTER);
    box->append(*frame);*/
    stack->add(*box, "overview", "Overview");
}

void MainWindow::prep_advanced(Gtk::Stack* stack) {
    Gtk::Box* box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 0);
    stack->add(*box, "advanced", "Advanced");
}

//-----------------------------Events:-----------------------------
void MainWindow::on_inspector_clicked() {
    viewMoreBtn->popdown();
    gtk_window_set_interactive_debugging(true);
}
}  // namespace ui::windows
