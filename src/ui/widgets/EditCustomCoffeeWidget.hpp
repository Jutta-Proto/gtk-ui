#pragma once

#include "backend/CoffeeMakerWrapper.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include <memory>
#include <gtkmm.h>
#include <gtkmm/enums.h>
#include <gtkmm/frame.h>

namespace ui::widgets {
class EditCustomCoffeeWidget : public Gtk::Frame {
 private:
    Gtk::Box* mainBox{nullptr};
    Gtk::Scale* waterScale{nullptr};
    Gtk::Scale* beansScale{nullptr};
    Gtk::Scale* tempScale{nullptr};

    backend::storage::UserProfile* profile{nullptr};

    using type_signal_clicked = sigc::signal<void>;
    type_signal_clicked m_signal_back_clicked;

    using type_signal_profile_value_changed = sigc::signal<void, backend::storage::UserProfile*>;
    type_signal_profile_value_changed m_signal_profile_value_changed;

 public:
    EditCustomCoffeeWidget();

    type_signal_clicked signal_back_clicked();
    type_signal_profile_value_changed signal_profile_value_changed();
    void set_user_profile(backend::storage::UserProfile* profile);

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_back_clicked();
    void on_reset_clicked();
    bool on_water_scale_button_released(GdkEventButton* event);
    bool on_beans_scale_button_released(GdkEventButton* event);
    bool on_temp_scale_button_released(GdkEventButton* event);
};
}  // namespace ui::widgets
