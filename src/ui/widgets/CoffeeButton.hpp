#pragma once

#include <jutta_proto/CoffeeMaker.hpp>
#include <gtkmm.h>

namespace ui::widgets {
class CoffeeButton : public Gtk::Button {
 public:
    /**
     * Signal for once the button has been clicked returning the sender.
     **/
    using type_signal_clicked_sender = sigc::signal<void, jutta_proto::CoffeeMaker::coffee_t>;

 private:
    jutta_proto::CoffeeMaker::coffee_t coffee;

    type_signal_clicked_sender m_signal_clicked_sender{};
    void on_button_clicked_wrapper();

 public:
    CoffeeButton(const Glib::ustring& label, jutta_proto::CoffeeMaker::coffee_t coffee, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);
    CoffeeButton(CoffeeButton&& src) noexcept;
    CoffeeButton& operator=(CoffeeButton&& src) noexcept;

    // noncopyable
    CoffeeButton(const CoffeeButton&) = delete;
    CoffeeButton& operator=(const CoffeeButton&) = delete;

    ~CoffeeButton() noexcept override = default;

    type_signal_clicked_sender signal_clicked_sender();

 private:
    void prep_button(const Glib::ustring& label, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);
};
}  // namespace ui::widgets
