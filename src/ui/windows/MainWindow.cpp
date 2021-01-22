#include "MainWindow.hpp"
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/object.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/stackswitcher.h>

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
    Gtk::MenuButton* viewMoreBtn = Gtk::make_managed<Gtk::MenuButton>();
    viewMoreBtn->set_icon_name("open-menu");
    Gtk::PopoverMenu* viewMorePopover = Gtk::make_managed<Gtk::PopoverMenu>();
    Gtk::Box* viewMoreMenuBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    Gtk::Button* settingsBtn = Gtk::make_managed<Gtk::Button>("Settings");
    viewMoreMenuBox->append(*settingsBtn);
    Gtk::Button* inspectorBtn = Gtk::make_managed<Gtk::Button>("Inspector");
    viewMoreMenuBox->append(*inspectorBtn);
    Gtk::Button* aboutBtn = Gtk::make_managed<Gtk::Button>();
    viewMoreMenuBox->append(*aboutBtn);
    viewMorePopover->set_child(*viewMoreMenuBox);
    viewMoreBtn->set_popover(*viewMorePopover);
    headerBar->pack_end(*viewMoreBtn);

    Gtk::StackSwitcher* stackSwitcher = Gtk::make_managed<Gtk::StackSwitcher>();
    stackSwitcher->set_stack(*stack);
    stackSwitcher->add_css_class("suggested-action");
    headerBar->set_title_widget(*stackSwitcher);

    set_titlebar(*headerBar);
}

void MainWindow::prep_overview(Gtk::Stack* stack) {
    Gtk::Box* box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    box->set_halign(Gtk::Align::FILL);
    box->set_valign(Gtk::Align::FILL);
    box->set_vexpand(true);
    box->set_homogeneous();
    Gtk::Frame* frame = Gtk::make_managed<Gtk::Frame>();
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("OLAF");
    frame->set_child(*label);
    frame->set_halign(Gtk::Align::CENTER);
    frame->set_valign(Gtk::Align::CENTER);
    box->append(*frame);
    stack->add(*box, "overview", "Overview");

    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_data("box {color: yellow; background: green;}");
    box->get_style_context()->add_provider(cssProvider, 100);
}

void MainWindow::prep_advanced(Gtk::Stack* stack) {
    Gtk::Box* box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 0);
    stack->add(*box, "advanced", "Advanced");
}

//-----------------------------Events:-----------------------------
void MainWindow::on_more_clicked() {
}
}  // namespace ui::windows
