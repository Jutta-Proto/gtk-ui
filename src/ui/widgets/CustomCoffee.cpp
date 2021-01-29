#include "CustomCoffee.hpp"
#include <gtkmm/adjustment.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/scale.h>

namespace ui::widgets {
CustomCoffee::CustomCoffee() : Gtk::Box(Gtk::Orientation::HORIZONTAL) {
    prep_widget();
}

void CustomCoffee::prep_widget() {
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));
    get_style_context()->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    add_css_class("custom-coffee-box");

    set_halign(Gtk::Align::CENTER);
    set_margin(10);

    // Scales:
    Gtk::Box* scalesBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    scalesBox->set_margin_start(10);
    scalesBox->set_margin_top(5);
    scalesBox->set_size_request(300, -1);
    Gtk::Label* waterLabel = Gtk::make_managed<Gtk::Label>("Water:");
    waterLabel->set_halign(Gtk::Align::START);
    scalesBox->append(*waterLabel);
    Gtk::Scale* waterScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(100, 50, 200), Gtk::Orientation::HORIZONTAL);
    waterScale->set_digits(0);
    waterScale->set_draw_value();
    scalesBox->append(*waterScale);
    Gtk::Label* beansLabel = Gtk::make_managed<Gtk::Label>("Beans:");
    beansLabel->set_halign(Gtk::Align::START);
    scalesBox->append(*beansLabel);
    Gtk::Scale* beansScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(100, 50, 200), Gtk::Orientation::HORIZONTAL);
    beansScale->set_digits(0);
    beansScale->set_draw_value();
    scalesBox->append(*beansScale);
    append(*scalesBox);

    // Brew button:
    Gtk::Button* brewBtn = Gtk::make_managed<Gtk::Button>("Brew");
    brewBtn->get_style_context()->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    brewBtn->add_css_class("coffee-button");
    brewBtn->set_margin(5);
    brewBtn->set_size_request(50, 50);
    brewBtn->set_halign(Gtk::Align::CENTER);
    brewBtn->set_valign(Gtk::Align::CENTER);
    append(*brewBtn);
}
}  // namespace ui::widgets
