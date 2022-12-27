#include "CoffeeMakerStatusWidget.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <gtkmm/button.h>
#include <gtkmm/popover.h>

namespace ui::widgets {
CoffeeMakerStatusWidget::CoffeeMakerStatusWidget() : statusPopover(*this) {
    prep_button();
    signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerStatusWidget::on_btn_clicked));
    alertsChangedDisp.connect(sigc::mem_fun(*this, &CoffeeMakerStatusWidget::on_alerts_changed));
}

void CoffeeMakerStatusWidget::prep_button() {
    // Content
    set_label("✅");
    set_visible(false);
    statusPopover.add(statusPopoverLabel);
    statusPopoverLabel.set_margin_start(5);
    statusPopoverLabel.set_margin_end(5);
    statusPopoverLabel.set_margin_top(5);
    statusPopoverLabel.set_margin_bottom(5);
    statusPopoverLabel.show_all();

    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("coffee-button");
}

void CoffeeMakerStatusWidget::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) {
    if (alertsHandle) {
        this->coffeeMaker->get_joe()->alertsChangedEventHandler.remove(*alertsHandle);
        alertsHandle = std::nullopt;
    }

    this->coffeeMaker = std::move(coffeeMaker);
    if (this->coffeeMaker) {
        alertsHandle = this->coffeeMaker->get_joe()->alertsChangedEventHandler.append([this](const std::vector<const jutta_bt_proto::Alert*>& /*alerts*/) { this->alertsChangedDisp.emit(); });
    }
    on_alerts_changed();
}

//-----------------------------Events:-----------------------------
void CoffeeMakerStatusWidget::on_alerts_changed() {
    if (!coffeeMaker) {
        this->set_visible(false);
        return;
    }

    size_t blockingCount = 0;
    size_t miscCount = 0;
    std::string statusBlockingText = "<span font_weight='bold'>Blocking:</span>";
    std::string statusMiscText = "<span font_weight='bold'>Misc:</span>";
    for (const jutta_bt_proto::Alert* alert : coffeeMaker->get_alerts()) {
        assert(alert);
        if (alert->type == "block") {
            blockingCount++;
            statusBlockingText += "\n• " + alert->name;
        } else {
            miscCount++;
            statusMiscText += "\n• " + alert->name;
        }
    }

    if (blockingCount > 0) {
        this->set_label(std::to_string(blockingCount) + " ❌");
        this->set_visible(true);
    } else if (miscCount > 0) {
        this->set_label(std::to_string(miscCount) + " ⚠️");
        this->set_visible(true);
    } else {
        this->set_label("✅");
        this->set_visible(false);
    }

    std::string statusText;
    if (blockingCount > 0) {
        statusText += statusBlockingText;
    }

    if (miscCount > 0) {
        if (!statusText.empty()) {
            statusText += "\n\n";
        }
        statusText += statusMiscText;
    }

    set_sensitive(!statusText.empty());
    statusPopoverLabel.set_markup(statusText);
}

void CoffeeMakerStatusWidget::on_btn_clicked() {
    statusPopover.show();
}
}  // namespace ui::widgets