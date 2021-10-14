#pragma once

#include "CoffeeButton.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include "jutta_bt_proto/CoffeeMakerLoader.hpp"
#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <gtkmm.h>
#include <gtkmm/flowbox.h>

namespace ui::widgets {
class CoffeeSelectionWidget : public Gtk::FlowBox {
 private:
    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker{nullptr};
    backend::storage::UserProfile* profile{nullptr};

    using type_signal_edit_custom_coffee_clicked = sigc::signal<void>;
    type_signal_edit_custom_coffee_clicked m_signal_edit_custom_coffee_clicked;

 public:
    CoffeeSelectionWidget();

    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);
    void set_user_profile(backend::storage::UserProfile* profile);

    type_signal_edit_custom_coffee_clicked signal_edit_custom_coffee_clicked();

 private:
    void prep_widget();
    CoffeeButton& generate_button(const std::string& name, const jutta_bt_proto::Product* product, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);
    void add_custom_coffee_buttons(const Glib::RefPtr<Gtk::CssProvider>& cssProvider);

    //-----------------------------Events:-----------------------------
    void on_coffee_button_clicked(const jutta_bt_proto::Product* product);
    void on_brew_custom_coffee_clicked(const jutta_bt_proto::Product* product);
    void on_edit_custom_coffee_clicked(const jutta_bt_proto::Product* product);
};
}  // namespace ui::widgets
