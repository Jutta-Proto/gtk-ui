#pragma once

#include "backend/CoffeeMakerWrapper.hpp"
#include <memory>
#include <gtkmm.h>
#include <gtkmm/frame.h>

namespace ui::widgets {
class CustomCoffeeWidget : public Gtk::Box {
 private:
    Gtk::Scale* waterScale{nullptr};
    Gtk::Scale* beansScale{nullptr};

    std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker{nullptr};

    bool cancel{false};

 public:
    CustomCoffeeWidget();

    void set_coffee_maker(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker);

 private:
    void prep_widget();
    static Gtk::FlowBoxChild& generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);

    //-----------------------------Events:-----------------------------
    void on_brew_clicked();
};
}  // namespace ui::widgets
