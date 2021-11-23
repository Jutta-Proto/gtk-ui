#include "CoffeeMakerButton.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/popover.h>

namespace ui::widgets {
CoffeeMakerButton::CoffeeMakerButton() : mainBox(Gtk::Orientation::ORIENTATION_VERTICAL),
                                         popoverMainBox(Gtk::Orientation::ORIENTATION_VERTICAL),
                                         statusPopover(*this) {
    prep_button();
    signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerButton::on_btn_clicked));
}

void CoffeeMakerButton::prep_button() {
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleCtx->add_class("coffee-button");

    // Content:
    this->add(mainBox);
    imageBuf = Gdk::Pixbuf::create_from_resource("/ui/coffee_maker.png", 20, 20, true);
    image = Gtk::make_managed<Gtk::Image>(imageBuf);
    image->set_halign(Gtk::Align::ALIGN_CENTER);
    mainBox.add(*image);

    // Popover:
    statusPopover.add(popoverMainBox);

    // Reconnect:
    popoverMainBox.add(reconnectBtn);
    reconnectBtn.set_label("Reconnect");
    popoverMainBox.show_all();
}

void CoffeeMakerButton::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) { this->coffeeMaker = std::move(coffeeMaker); }

//-----------------------------Events:-----------------------------
void CoffeeMakerButton::on_btn_clicked() {
    statusPopover.show();
}
}  // namespace ui::widgets