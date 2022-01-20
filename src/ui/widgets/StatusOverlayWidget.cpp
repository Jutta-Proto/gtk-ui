#include "StatusOverlayWidget.hpp"
#include "eventpp/callbacklist.h"
#include "ui/utils/UiUtils.hpp"
#include <optional>
#include <gtkmm/enums.h>

namespace ui::widgets {
StatusOverlayWidget::StatusOverlayWidget() {
    prep_widget();
    alertsChangedDisp.connect(sigc::mem_fun(*this, &StatusOverlayWidget::on_alerts_changed));
}

void StatusOverlayWidget::prep_widget() {
    // General:
    set_valign(Gtk::ALIGN_CENTER);
    set_halign(Gtk::ALIGN_CENTER);
    set_vexpand(true);
    set_shadow_type(Gtk::ShadowType::SHADOW_OUT);
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("overlay-frame-background");

    Gtk::Box* mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_HORIZONTAL, 0);
    mainBox->set_margin_bottom(10);
    mainBox->set_margin_start(10);
    mainBox->set_margin_top(10);
    mainBox->set_margin_end(10);
    mainBox->set_halign(Gtk::Align::ALIGN_CENTER);
    add(*mainBox);

    Gtk::Image* warningImage = Gtk::make_managed<Gtk::Image>();
    warningImage->set_from_icon_name("dialog-error", Gtk::ICON_SIZE_DIALOG);
    mainBox->add(*warningImage);

    // Seperator:
    Gtk::Separator* seperator = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::ORIENTATION_VERTICAL);
    seperator->set_margin_bottom(10);
    seperator->set_margin_start(10);
    seperator->set_margin_top(10);
    seperator->set_margin_end(10);
    seperator->set_valign(Gtk::Align::ALIGN_FILL);
    mainBox->add(*seperator);

    // Status:
    mainBox->add(statusLabel);
    statusLabel.set_valign(Gtk::Align::ALIGN_CENTER);

    show_all();
}

void StatusOverlayWidget::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) {
    if (alertsHandle) {
        this->coffeeMaker->alertsChangedEventHandler.remove(*alertsHandle);
        alertsHandle = std::nullopt;
    }

    this->coffeeMaker = std::move(coffeeMaker);
    if (this->coffeeMaker) {
        alertsHandle = this->coffeeMaker->alertsChangedEventHandler.append([this](const std::vector<const jutta_bt_proto::Alert*>& /*alerts*/) { this->alertsChangedDisp.emit(); });
    }
    on_alerts_changed();
}

//-----------------------------Events:-----------------------------
void StatusOverlayWidget::on_alerts_changed() {
    size_t blockingCount = 0;
    std::string statusText = "<span font_weight='bold'>Blocking:</span>";
    if (coffeeMaker) {
        for (const jutta_bt_proto::Alert* alert : coffeeMaker->get_alerts()) {
            assert(alert);
            if (alert->type == "block") {
                blockingCount++;
                statusText += "\n• " + alert->name;
            }
        }
    } else {
        statusText += "\nNot connected";
    }
    statusLabel.set_markup(statusText);
}

}  // namespace ui::widgets
