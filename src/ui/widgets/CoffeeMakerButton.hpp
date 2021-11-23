#pragma once

#include <jutta_bt_proto/CoffeeMaker.hpp>
#include <jutta_bt_proto/CoffeeMakerLoader.hpp>
#include <memory>
#include <vector>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>

namespace ui::widgets {
class CoffeeMakerButton : public Gtk::Button {
 private:
    Glib::RefPtr<Gdk::Pixbuf> imageBuf{nullptr};
    Gtk::Image* image{nullptr};
    Gtk::Box mainBox;
    Gtk::Box popoverMainBox;
    Gtk::Popover statusPopover;
    Gtk::Button reconnectBtn{};

    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker{nullptr};

 public:
    CoffeeMakerButton();
    CoffeeMakerButton(CoffeeMakerButton&& src) noexcept = delete;
    CoffeeMakerButton& operator=(CoffeeMakerButton&& src) noexcept = delete;

    ~CoffeeMakerButton() noexcept override = default;

    // noncopyable
    CoffeeMakerButton(const CoffeeMakerButton&) = delete;
    CoffeeMakerButton& operator=(const CoffeeMakerButton&) = delete;

    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);

 private:
    void prep_button();

    //-----------------------------Events:-----------------------------
    void on_btn_clicked();
};
}  // namespace ui::widgets
