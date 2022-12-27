#pragma once

#include "CoffeeMakerButton.hpp"
#include "CoffeeMakerLockButton.hpp"
#include "CoffeeMakerStatusWidget.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <string>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <sigc++-2.0/sigc++/signal.h>

namespace ui::widgets {
class StatusBarWidget : public Gtk::Box {
 private:
    Gtk::Button logoutBtn{};
    Gtk::Label userLabel{};
    Gtk::Label userIdLabel{};
    CoffeeMakerStatusWidget statusWidget{};
    CoffeeMakerButton coffeeMakerButton{};
    CoffeeMakerLockButton coffeeMakerLockButton{};

    backend::storage::UserProfile* profile{nullptr};

    using type_signal_clicked = sigc::signal<void>;
    type_signal_clicked m_signal_logout_clicked;
    type_signal_clicked m_signal_disconnect_clicked;

 public:
    StatusBarWidget();

    void set_user_profile(backend::storage::UserProfile* profile);
    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);
    type_signal_clicked signal_logout_clicked();
    type_signal_clicked signal_disconnect_clicked();

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_logout_clicked();
    void on_disconnect_clicked();
};
}  // namespace ui::widgets
