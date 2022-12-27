#pragma once

#include "jutta_bt_proto/CoffeeMaker.hpp"
#include <memory>
#include <gtkmm.h>

namespace ui::widgets {
class CoffeeMakerLockButton : public Gtk::Button {
 public:
    /**
     * Signal for once the button has been clicked with a bool indicating wether the coffee maker locked.
     **/
    using type_signal_clicked_sender = sigc::signal<void, bool>;

 private:
    bool locked{false};
    std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker;

    type_signal_clicked_sender m_signal_clicked_sender{};

 public:
    CoffeeMakerLockButton();
    CoffeeMakerLockButton(CoffeeMakerLockButton&& src) noexcept = delete;
    CoffeeMakerLockButton& operator=(CoffeeMakerLockButton&& src) noexcept;

    CoffeeMakerLockButton(const CoffeeMakerLockButton&) = delete;
    CoffeeMakerLockButton& operator=(const CoffeeMakerLockButton&) = delete;

    ~CoffeeMakerLockButton() noexcept override = default;

    type_signal_clicked_sender signal_clicked_sender();

    void set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker);

 private:
    void prep_button();
    void update_button_state();

    //-----------------------------Events:-----------------------------
    void on_button_clicked_wrapper();
};
}  // namespace ui::widgets
