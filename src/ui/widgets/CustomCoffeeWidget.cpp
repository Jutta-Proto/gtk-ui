#include "CustomCoffeeWidget.hpp"
#include <cassert>
#include <bits/stdint-intn.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <chrono>
#include <gtkmm/scale.h>

namespace ui::widgets {
CustomCoffeeWidget::CustomCoffeeWidget() : Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL) {
    prep_widget();
}

void CustomCoffeeWidget::prep_widget() {
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
    waterScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(100, 50, 200), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    waterScale->set_digits(0);
    waterScale->set_draw_value();
    scalesBox->add(*waterScale);
    Gtk::Label* beansLabel = Gtk::make_managed<Gtk::Label>("Beans:");
    beansLabel->set_halign(Gtk::Align::ALIGN_START);
    scalesBox->add(*beansLabel);
    beansScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(100, 50, 200), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    beansScale->set_digits(0);
    beansScale->set_draw_value();
    scalesBox->add(*beansScale);
    add(*scalesBox);

    // Brew button:
    Gtk::Button* brewBtn = Gtk::make_managed<Gtk::Button>("Brew");
    brewBtn->signal_clicked().connect(sigc::mem_fun(this, &CustomCoffeeWidget::on_brew_clicked));
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

void CustomCoffeeWidget::set_coffee_maker(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
}

//-----------------------------Events:-----------------------------
void CustomCoffeeWidget::on_brew_clicked() {
    assert(coffeeMaker);
    set_sensitive(false);
    std::chrono::milliseconds beansTime = std::chrono::milliseconds{static_cast<int64_t>(3600.0 * (beansScale->get_value() / 100.0))};
    std::chrono::milliseconds waterTime = std::chrono::milliseconds{static_cast<int64_t>(40000.0 * (beansScale->get_value() / 100.0))};
    cancel = false;
    coffeeMaker->get_coffee_maker()->brew_custom_coffee(&cancel, beansTime, waterTime);
    set_sensitive(true);
}
}  // namespace ui::widgets
