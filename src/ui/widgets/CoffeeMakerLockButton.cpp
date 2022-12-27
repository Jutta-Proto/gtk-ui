#include "CoffeeMakerLockButton.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>

namespace ui::widgets {
CoffeeMakerLockButton::CoffeeMakerLockButton() {
    prep_button();
}

CoffeeMakerLockButton& CoffeeMakerLockButton::operator=(CoffeeMakerLockButton&& src) noexcept {
    Gtk::Button::operator=(std::move(src));
    return *this;
}

void CoffeeMakerLockButton::prep_button() {
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("coffee-button");

    signal_clicked().connect(sigc::mem_fun(this, &CoffeeMakerLockButton::on_button_clicked_wrapper));
    update_button_state();
}

CoffeeMakerLockButton::type_signal_clicked_sender CoffeeMakerLockButton::signal_clicked_sender() {
    return m_signal_clicked_sender;
}

void CoffeeMakerLockButton::update_button_state() {
    if (locked) {
        set_tooltip_text("LOCKED - Click to unlock coffee maker.");
        set_label("🔒");
    } else {
        set_tooltip_text("UNLOCKED - Click to lock coffee maker.");
        set_label("🔓");
    }
}

void CoffeeMakerLockButton::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
}

//-----------------------------Events:-----------------------------
void CoffeeMakerLockButton::on_button_clicked_wrapper() {
    locked = !locked;

    assert(coffeeMaker);
    if (locked) {
        coffeeMaker->lock();
    } else {
        coffeeMaker->unlock();
    }

    update_button_state();
    m_signal_clicked_sender.emit(locked);
}
}  // namespace ui::widgets