#include "CoffeeSelection.hpp"
#include <giomm/file.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>

namespace ui::widgets {
CoffeeSelection::CoffeeSelection() {
    prep_widget();
}

void CoffeeSelection::prep_widget() {
    set_selection_mode(Gtk::SelectionMode::NONE);

    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));

    // Add content:
    this->insert(generate_button("A0", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A1", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A2", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A3", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A4", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A5", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A6", "coffee-button-background", cssProvider), 0);
    this->insert(generate_button("A7", "coffee-button-background", cssProvider), 0);
}

Gtk::FlowBoxChild& CoffeeSelection::generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    Gtk::FlowBoxChild* child = Gtk::make_managed<Gtk::FlowBoxChild>();
    Gtk::Button* btn = Gtk::make_managed<Gtk::Button>(name);
    btn->set_size_request(150, 150);
    // btn->set_opacity(0.7);
    btn->get_style_context()->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    btn->add_css_class("coffee-button");
    btn->add_css_class(cssClass);
    btn->set_margin(10);
    child->set_child(*btn);
    return *child;
}
}  // namespace ui::widgets
