#pragma once

#include "backend/storage/UserProfileStorage.hpp"
#include <string>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <sigc++-2.0/sigc++/signal.h>

namespace ui::widgets {
class StatusBarWidget : public Gtk::Box {
 private:
    Gtk::Button logoutBtn{};
    Gtk::Button reconnectBtn{};
    Gtk::Label userLabel{};
    Gtk::Label userIdLabel{};

    backend::storage::UserProfile* profile{nullptr};

    using type_signal_clicked = sigc::signal<void>;
    type_signal_clicked m_signal_logout_clicked;
    type_signal_clicked m_signal_reconnect_clicked;

 public:
    StatusBarWidget();

    void set_user_profile(backend::storage::UserProfile* profile);
    type_signal_clicked signal_logout_clicked();
    type_signal_clicked signal_reconnect_clicked();

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_logout_clicked();
    void on_reconnect_clicked();
};
}  // namespace ui::widgets
