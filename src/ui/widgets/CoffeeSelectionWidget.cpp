#include "CoffeeSelectionWidget.hpp"
#include <giomm/file.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>

namespace ui::widgets {
CoffeeSelectionWidget::CoffeeSelectionWidget() {
    prep_widget();
}

void CoffeeSelectionWidget::prep_widget() {
    set_selection_mode(Gtk::SelectionMode::SELECTION_NONE);

    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));

    // Add content:
    this->add(generate_button("Coffee", "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Espresso", "coffee-button-espresso-background", cssProvider));
    this->add(generate_button("Cappuccino", "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Milk foam", "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Caffe Barista", "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Lungo Barista", "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Espresso doppio", "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Macciato", "coffee-button-coffee-background", cssProvider));
}

Gtk::Button& CoffeeSelectionWidget::generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    Gtk::Button* btn = Gtk::make_managed<Gtk::Button>(name);
    btn->set_size_request(100, 50);
    Glib::RefPtr<Gtk::StyleContext> styleCtx = btn->get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleCtx->add_class("coffee-button");
    styleCtx->add_class(cssClass);
    btn->set_margin_bottom(10);
    btn->set_margin_top(10);
    btn->set_margin_left(10);
    btn->set_margin_right(10);
    return *btn;
}
}  // namespace ui::widgets
