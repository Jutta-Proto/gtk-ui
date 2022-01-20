#pragma once

#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <memory>
#include <optional>
#include <vector>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>

namespace ui::widgets {
class CoffeeMakerStatusWidget : public Gtk::Button {
 private:
    Gtk::Label statusPopoverLabel{};
    Gtk::Popover statusPopover;

    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker{nullptr};
    Glib::Dispatcher alertsChangedDisp;
    std::optional<eventpp::CallbackList<void(const std::vector<const jutta_bt_proto::Alert*>&)>::Handle> alertsHandle;

 public:
    CoffeeMakerStatusWidget();
    CoffeeMakerStatusWidget(CoffeeMakerStatusWidget&& src) noexcept = delete;
    CoffeeMakerStatusWidget& operator=(CoffeeMakerStatusWidget&& src) noexcept = delete;

    ~CoffeeMakerStatusWidget() noexcept override = default;

    // noncopyable
    CoffeeMakerStatusWidget(const CoffeeMakerStatusWidget&) = delete;
    CoffeeMakerStatusWidget& operator=(const CoffeeMakerStatusWidget&) = delete;

    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);

 private:
    void prep_button();

    //-----------------------------Events:-----------------------------
    void on_alerts_changed();
    void on_btn_clicked();
};
}  // namespace ui::widgets
