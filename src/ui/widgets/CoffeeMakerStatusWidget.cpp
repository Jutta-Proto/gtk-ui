#include "CoffeeMakerStatusWidget.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <gtkmm/button.h>
#include <gtkmm/popover.h>

namespace ui::widgets {
CoffeeMakerStatusWidget::CoffeeMakerStatusWidget() : statusPopover(*this) {
    prep_button();
    signal_clicked().connect(sigc::mem_fun(*this, &CoffeeMakerStatusWidget::on_btn_clicked));
}

void CoffeeMakerStatusWidget::prep_button() {
    // Content
    set_label("✅");
    statusPopover.add(statusPopoverLabel);
    statusPopoverLabel.set_margin_start(5);
    statusPopoverLabel.set_margin_end(5);
    statusPopoverLabel.set_margin_top(5);
    statusPopoverLabel.set_margin_bottom(5);
    statusPopoverLabel.show_all();
}

void CoffeeMakerStatusWidget::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) {
    this->coffeeMaker = std::move(coffeeMaker);
    if (this->coffeeMaker) {
        this->coffeeMaker->set_alerts_changed_event_handler([this](const std::vector<const jutta_bt_proto::Alert*>& alerts) { this->on_alerts_changed(alerts); });
        on_alerts_changed(this->coffeeMaker->get_alerts());
    }
}

//-----------------------------Events:-----------------------------
void CoffeeMakerStatusWidget::on_alerts_changed(const std::vector<const jutta_bt_proto::Alert*>& alerts) {
    size_t blockingCount = 0;
    size_t miscCount = 0;
    std::string statusBlockingText = "<span font_weight='bold'>Blocking:</span>";
    std::string statusMiscText = "<span font_weight='bold'>Misc:</span>";
    for (const jutta_bt_proto::Alert* alert : alerts) {
        assert(alert);
        continue;
        if (alert->type == "block") {
            blockingCount++;
            statusBlockingText += "\n* " + alert->name;
        } else {
            miscCount++;
            statusMiscText += "\n* " + alert->name;
        }
    }

    if (blockingCount > 0) {
        this->set_label(std::to_string(blockingCount) + "❌");
    } else if (miscCount > 0) {
        this->set_label(std::to_string(blockingCount) + "⚠️");
    } else {
        this->set_label("✅");
    }

    std::string statusText;
    if (blockingCount > 0) {
        statusText += statusBlockingText;
    }

    if (miscCount > 0) {
        if (!statusText.empty()) {
            statusText += '\n';
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