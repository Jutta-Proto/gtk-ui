#include "CoffeeSelectionWidget.hpp"
#include <giomm/file.h>
#include <gtkmm/enums.h>
#include <sigc++/functors/mem_fun.h>
#include <cassert>

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
    this->add(generate_button("Coffee", jutta_proto::CoffeeMaker::coffee_t::COFFEE, "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Espresso", jutta_proto::CoffeeMaker::coffee_t::ESPRESSO, "coffee-button-espresso-background", cssProvider));
    this->add(generate_button("Cappuccino", jutta_proto::CoffeeMaker::coffee_t::CAPPUCCINO, "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Milk foam", jutta_proto::CoffeeMaker::coffee_t::MILK_FOAM, "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Caffe Barista", jutta_proto::CoffeeMaker::coffee_t::CAFFE_BARISTA, "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Lungo Barista", jutta_proto::CoffeeMaker::coffee_t::LUNGO_BARISTA, "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Espresso doppio", jutta_proto::CoffeeMaker::coffee_t::ESPRESSO_DOPPIO, "coffee-button-coffee-background", cssProvider));
    this->add(generate_button("Macciato", jutta_proto::CoffeeMaker::coffee_t::MACCHIATO, "coffee-button-coffee-background", cssProvider));

    add_custom_coffee_buttons(cssProvider);
}

void CoffeeSelectionWidget::set_user_profile(backend::storage::UserProfile* profile) {
    this->profile = profile;
}

void CoffeeSelectionWidget::add_custom_coffee_buttons(const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    CoffeeButton* brewCustom = Gtk::make_managed<CoffeeButton>("Custom Coffee", jutta_proto::CoffeeMaker::coffee_t::COFFEE, "coffee-button-coffee-background", cssProvider);
    brewCustom->signal_clicked_sender().connect(sigc::mem_fun(this, &CoffeeSelectionWidget::on_brew_custom_coffee_clicked));
    this->add(*brewCustom);
    CoffeeButton* editCustom = Gtk::make_managed<CoffeeButton>("Edit\nCustom Coffee", jutta_proto::CoffeeMaker::coffee_t::COFFEE, "coffee-button-coffee-background", cssProvider);
    editCustom->signal_clicked_sender().connect(sigc::mem_fun(this, &CoffeeSelectionWidget::on_edit_custom_coffee_clicked));
    this->add(*editCustom);
}

CoffeeButton& CoffeeSelectionWidget::generate_button(const std::string& name, jutta_proto::CoffeeMaker::coffee_t coffee, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    CoffeeButton* btn = Gtk::make_managed<CoffeeButton>(name, coffee, cssClass, cssProvider);
    btn->signal_clicked_sender().connect(sigc::mem_fun(this, &CoffeeSelectionWidget::on_coffee_button_clicked));
    return *btn;
}

void CoffeeSelectionWidget::set_coffee_maker(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
}

//-----------------------------Events:-----------------------------
void CoffeeSelectionWidget::on_coffee_button_clicked(jutta_proto::CoffeeMaker::coffee_t coffee) {
    assert(coffeeMaker);
    coffeeMaker->get_coffee_maker()->brew_coffee(coffee);
}

void CoffeeSelectionWidget::on_brew_custom_coffee_clicked(jutta_proto::CoffeeMaker::coffee_t /*coffee*/) {
    assert(coffeeMaker);
    bool cancel = false;
    std::chrono::milliseconds beansTime = std::chrono::milliseconds{static_cast<int64_t>(3600.0 * profile->beansFactor)};
    std::chrono::milliseconds waterTime = std::chrono::milliseconds{static_cast<int64_t>(40000.0 * profile->waterFactor)};
    coffeeMaker->get_coffee_maker()->brew_custom_coffee(&cancel, beansTime, waterTime);
}

void CoffeeSelectionWidget::on_edit_custom_coffee_clicked(jutta_proto::CoffeeMaker::coffee_t /*coffee*/) {
    assert(coffeeMaker);
}
}  // namespace ui::widgets
