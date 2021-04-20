#pragma once

#include "backend/CoffeeMakerWrapper.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include <memory>
#include <gtkmm.h>
#include <gtkmm/enums.h>
#include <gtkmm/frame.h>

namespace ui::widgets {
class EditCustomCoffeeWidget : public Gtk::Box {
 private:
    Gtk::Scale* waterScale{nullptr};
    Gtk::Scale* beansScale{nullptr};

    std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker{nullptr};
    backend::storage::UserProfile* profile{nullptr};

    bool cancel{false};

 public:
    EditCustomCoffeeWidget();

    void set_coffee_maker(std::shared_ptr<backend::CoffeeMakerWrapper> coffeeMaker);
    void set_user_profile(backend::storage::UserProfile* profile);

 private:
    void prep_widget();
    static Gtk::FlowBoxChild& generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);

    //-----------------------------Events:-----------------------------
    void on_brew_clicked();
    bool on_water_scale_button_released(GdkEventButton* event);
    bool on_beans_scale_button_released(GdkEventButton* event);
};
}  // namespace ui::widgets
