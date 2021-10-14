#include "CoffeeButton.hpp"
#include <gtkmm/box.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>

namespace ui::widgets {
CoffeeButton::CoffeeButton(const Glib::ustring& label, const jutta_bt_proto::Product* product, const std::string& imageResourcePath, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) : product(product) {
    prep_button(label, imageResourcePath, cssProvider);
}

CoffeeButton::CoffeeButton(CoffeeButton&& src) noexcept : Gtk::Button(std::move(src)), product(src.product) {
    signal_clicked().connect(sigc::mem_fun(this, &CoffeeButton::on_button_clicked_wrapper));
}

CoffeeButton& CoffeeButton::operator=(CoffeeButton&& src) noexcept {
    product = src.product;
    Gtk::Button::operator=(std::move(src));
    return *this;
}

void CoffeeButton::prep_button(const Glib::ustring& label, const std::string& imageResourcePath, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    signal_clicked().connect(sigc::mem_fun(this, &CoffeeButton::on_button_clicked_wrapper));
    set_size_request(75, 125);

    // Style:
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleCtx->add_class("coffee-button");

    // Content:
    contentBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    this->add(*contentBox);

    imageBuf = Gdk::Pixbuf::create_from_resource(imageResourcePath, 75, 75, true);
    image = Gtk::make_managed<Gtk::Image>(imageBuf);
    image->set_halign(Gtk::Align::ALIGN_CENTER);
    contentBox->add(*image);

    nameLabel = Gtk::make_managed<Gtk::Label>();
    nameLabel->set_margin_top(10);
    nameLabel->set_justify(Gtk::Justification::JUSTIFY_CENTER);
    nameLabel->set_markup("<span font_weight='bold'>" + label + "</span>");
    contentBox->add(*nameLabel);
}

CoffeeButton::type_signal_clicked_sender CoffeeButton::signal_clicked_sender() {
    return m_signal_clicked_sender;
}

void CoffeeButton::on_button_clicked_wrapper() {
    m_signal_clicked_sender.emit(product);
}
}  // namespace ui::widgets