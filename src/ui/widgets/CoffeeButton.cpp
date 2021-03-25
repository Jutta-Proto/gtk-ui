#include "CoffeeButton.hpp"

namespace ui::widgets {
CoffeeButton::CoffeeButton(const Glib::ustring& label, jutta_proto::CoffeeMaker::coffee_t coffee, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) : Gtk::Button(label), coffee(coffee) {
    prep_button(cssClass, cssProvider);
}

CoffeeButton::CoffeeButton(CoffeeButton&& src) noexcept : Gtk::Button(std::move(src)), coffee(src.coffee) {
    signal_clicked().connect(sigc::mem_fun(this, &CoffeeButton::on_button_clicked_wrapper));
}

CoffeeButton& CoffeeButton::operator=(CoffeeButton&& src) noexcept {
    coffee = src.coffee;
    Gtk::Button::operator=(std::move(src));
    return *this;
}

void CoffeeButton::prep_button(const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    signal_clicked().connect(sigc::mem_fun(this, &CoffeeButton::on_button_clicked_wrapper));

    set_size_request(100, 50);
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleCtx->add_class("coffee-button");
    styleCtx->add_class(cssClass);
    set_margin_bottom(10);
    set_margin_top(10);
    set_margin_start(10);
    set_margin_end(10);
}

CoffeeButton::type_signal_clicked_sender CoffeeButton::signal_clicked_sender() {
    return m_signal_clicked_sender;
}

void CoffeeButton::on_button_clicked_wrapper() {
    m_signal_clicked_sender.emit(coffee);
}
}  // namespace ui::widgets