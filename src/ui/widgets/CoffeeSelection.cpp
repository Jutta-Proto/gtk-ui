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
    this->insert(generate_button("Coffee", "coffee-button-coffee-background", cssProvider), 0);
    this->insert(generate_button("Espresso", "coffee-button-espresso-background", cssProvider), 1);
    this->insert(generate_button("Cappuccino", "coffee-button-coffee-background", cssProvider), 2);
    this->insert(generate_button("Milk foam", "coffee-button-coffee-background", cssProvider), 3);
    this->insert(generate_button("Caffe Barista", "coffee-button-coffee-background", cssProvider), 4);
    this->insert(generate_button("Lungo Barista", "coffee-button-coffee-background", cssProvider), 5);
    this->insert(generate_button("Espresso doppio", "coffee-button-coffee-background", cssProvider), 6);
    this->insert(generate_button("Macciato", "coffee-button-coffee-background", cssProvider), 7);
}

Gtk::FlowBoxChild& CoffeeSelection::generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    Gtk::FlowBoxChild* child = Gtk::make_managed<Gtk::FlowBoxChild>();
    Gtk::Button* btn = Gtk::make_managed<Gtk::Button>(name);
    btn->set_size_request(150, 150);
    btn->get_style_context()->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    btn->add_css_class("coffee-button");
    btn->add_css_class(cssClass);
    btn->set_margin(10);
    child->set_child(*btn);
    return *child;
}
}  // namespace ui::widgets
