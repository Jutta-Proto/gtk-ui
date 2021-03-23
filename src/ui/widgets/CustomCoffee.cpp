#include "CustomCoffee.hpp"
#include <gtkmm/adjustment.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/scale.h>

namespace ui::widgets {
CustomCoffee::CustomCoffee() : Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL) {
    prep_widget();
}

void CustomCoffee::prep_widget() {
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_file(Gio::File::create_for_uri("resource:///ui/theme.css"));
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleCtx->add_class("custom-coffee-box");

    set_halign(Gtk::Align::ALIGN_CENTER);
    set_margin_bottom(10);
    set_margin_top(10);
    set_margin_left(10);
    set_margin_right(10);

    // Scales:
    Gtk::Box* scalesBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    scalesBox->set_margin_start(10);
    scalesBox->set_margin_top(5);
    scalesBox->set_size_request(300, -1);
    Gtk::Label* waterLabel = Gtk::make_managed<Gtk::Label>("Water:");
    waterLabel->set_halign(Gtk::Align::ALIGN_START);
    scalesBox->add(*waterLabel);
    Gtk::Scale* waterScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(100, 50, 200), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    waterScale->set_digits(0);
    waterScale->set_draw_value();
    scalesBox->add(*waterScale);
    Gtk::Label* beansLabel = Gtk::make_managed<Gtk::Label>("Beans:");
    beansLabel->set_halign(Gtk::Align::ALIGN_START);
    scalesBox->add(*beansLabel);
    Gtk::Scale* beansScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(100, 50, 200), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    beansScale->set_digits(0);
    beansScale->set_draw_value();
    scalesBox->add(*beansScale);
    add(*scalesBox);

    // Brew button:
    Gtk::Button* brewBtn = Gtk::make_managed<Gtk::Button>("Brew");
    styleCtx = brewBtn->get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleCtx->add_class("coffee-button");
    brewBtn->set_margin_bottom(5);
    brewBtn->set_margin_top(5);
    brewBtn->set_margin_right(5);
    brewBtn->set_margin_left(5);
    brewBtn->set_size_request(50, 50);
    brewBtn->set_halign(Gtk::Align::ALIGN_CENTER);
    brewBtn->set_valign(Gtk::Align::ALIGN_CENTER);
    add(*brewBtn);
}
}  // namespace ui::widgets
