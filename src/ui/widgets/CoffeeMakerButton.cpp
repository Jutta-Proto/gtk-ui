#include "CoffeeMakerButton.hpp"
#include "backend/CoffeeMakerConnectionHandler.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <gdkmm/device.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/popover.h>

namespace ui::widgets {
CoffeeMakerButton::CoffeeMakerButton() : popoverMainBox(Gtk::Orientation::ORIENTATION_VERTICAL),
                                         statusPopover(*this) {
    prep_button();
    signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerButton::on_btn_clicked));
    disconnectBtn.signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerButton::on_disconnect_clicked));
    backend::CoffeeMakerConnectionHandler::get_instance().signal_state_changed().connect(sigc::mem_fun(*this, &CoffeeMakerButton::on_connection_state_changed));
}

void CoffeeMakerButton::prep_button() {
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("coffee-button");

    // Content:
    this->add(statusOverlay);
    imageBuf = Gdk::Pixbuf::create_from_resource("/ui/coffee_maker.png", 20, 20, true);
    image = Gtk::make_managed<Gtk::Image>(imageBuf);
    image->set_halign(Gtk::Align::ALIGN_CENTER);
    statusOverlay.add(*image);
    statusOverlay.add_overlay(statusCircle);
    statusOverlay.set_overlay_pass_through(statusCircle, true);
    statusOverlay.show_all();
    statusOverlay.set_events(Gdk::EventMask());

    // Popover:
    statusPopover.add(popoverMainBox);

    // Disconnect:
    popoverMainBox.add(disconnectBtn);
    disconnectBtn.set_label("Disconnect");
    popoverMainBox.show_all();

    update_connection_state(backend::CoffeeMakerConnectionHandler::get_instance().get_state());
}

void CoffeeMakerButton::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) { this->coffeeMaker = std::move(coffeeMaker); }

void CoffeeMakerButton::update_connection_state(backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState state) {
    switch (state) {
        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::NOT_RUNNING:
            statusCircle.set_color(Gdk::RGBA("red"));
            break;

        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::SEARCHING:
            statusCircle.set_color(Gdk::RGBA("blue"));
            break;

        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::CONNECTING:
            statusCircle.set_color(Gdk::RGBA("orange"));
            break;

        case backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState::CONNECTED:
            statusCircle.set_color(Gdk::RGBA("green"));
            break;

        default:
            assert(false);  // Should not happen...
            break;
    }
}

CoffeeMakerButton::type_signal_clicked CoffeeMakerButton::signal_disconnect_clicked() {
    return m_signal_disconnect_clicked;
}

//-----------------------------Events:-----------------------------
void CoffeeMakerButton::on_btn_clicked() {
    statusPopover.show();
}

void CoffeeMakerButton::on_connection_state_changed(backend::CoffeeMakerConnectionHandler::CoffeeMakerConnectionHandlerState state) {
    update_connection_state(state);
}

void CoffeeMakerButton::on_disconnect_clicked() {
    statusPopover.hide();
    m_signal_disconnect_clicked.emit();
}
}  // namespace ui::widgets