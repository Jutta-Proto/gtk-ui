#pragma once

#include "CoffeeButton.hpp"
#include "backend/CoffeeMakerWrapper.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include <jutta_proto/CoffeeMaker.hpp>
#include <memory>
#include <gtkmm.h>
#include <gtkmm/flowbox.h>

namespace ui::widgets {
class CoffeeSelectionWidget : public Gtk::FlowBox {
 private:
    std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker{nullptr};
    backend::storage::UserProfile* profile;

 public:
    CoffeeSelectionWidget();

    void set_coffee_maker(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker);
    void set_user_profile(backend::storage::UserProfile* profile);

 private:
    void prep_widget();
    CoffeeButton& generate_button(const std::string& name, jutta_proto::CoffeeMaker::coffee_t coffee, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);
    void add_custom_coffee_buttons(const Glib::RefPtr<Gtk::CssProvider>& cssProvider);

    //-----------------------------Events:-----------------------------
    void on_coffee_button_clicked(jutta_proto::CoffeeMaker::coffee_t coffee);
    void on_brew_custom_coffee_clicked(jutta_proto::CoffeeMaker::coffee_t coffee);
    void on_edit_custom_coffee_clicked(jutta_proto::CoffeeMaker::coffee_t coffee);
};
}  // namespace ui::widgets
