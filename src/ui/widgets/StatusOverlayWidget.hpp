#pragma once

#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <optional>
#include <string>
#include <gtkmm.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>

namespace ui::widgets {
class StatusOverlayWidget : public Gtk::Frame {
 private:
    Gtk::Label statusLabel{};
    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker{nullptr};
    Glib::Dispatcher alertsChangedDisp;
    std::optional<eventpp::CallbackList<void(const std::vector<const jutta_bt_proto::Alert*>&)>::Handle> alertsHandle;

 public:
    StatusOverlayWidget();

    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_alerts_changed();
};
}  // namespace ui::widgets
