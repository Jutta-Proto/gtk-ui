#include "StatusBarWidget.hpp"
#include "sigc++-2.0/sigc++/functors/mem_fun.h"
#include "ui/utils/UiUtils.hpp"
#include <gtkmm/enums.h>

namespace ui::widgets {
StatusBarWidget::StatusBarWidget() : Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL) {
    prep_widget();
}

void StatusBarWidget::set_user_profile(backend::storage::UserProfile* profile) {
    this->profile = profile;
    if (profile) {
        if (profile->cardId.empty()) {
            userIdLabel.set_label("default");
        } else {
            std::string userId = profile->cardId;
            if (userId.length() > 10) {
                userId = userId.substr(0, 10);
            }
            userIdLabel.set_label(userId);
        }
    } else {
        userIdLabel.set_label("-");
    }
}

StatusBarWidget::type_signal_clicked StatusBarWidget::signal_logout_clicked() {
    return m_signal_logout_clicked;
}

StatusBarWidget::type_signal_clicked StatusBarWidget::signal_reconnect_clicked() {
    return m_signal_reconnect_clicked;
}

void StatusBarWidget::prep_widget() {
    set_halign(Gtk::Align::ALIGN_FILL);
    set_valign(Gtk::Align::ALIGN_END);

    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("status-bar-background");

    // Logout:
    logoutBtn.set_label("Logout");
    logoutBtn.set_margin_end(10);
    logoutBtn.set_margin_start(10);
    logoutBtn.set_margin_top(10);
    logoutBtn.set_margin_bottom(10);
    logoutBtn.set_valign(Gtk::Align::ALIGN_CENTER);
    logoutBtn.signal_clicked().connect(sigc::mem_fun(this, &StatusBarWidget::on_logout_clicked));
    Glib::RefPtr<Gtk::StyleContext> logoutBtnStyleCtx = logoutBtn.get_style_context();
    logoutBtnStyleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    logoutBtnStyleCtx->add_class("coffee-button");
    add(logoutBtn);

    // User:
    userLabel.set_selectable(true);
    userLabel.set_valign(Gtk::Align::ALIGN_CENTER);
    userLabel.set_markup("<span font_weight='bold'>User:</span>");
    userLabel.set_margin_end(10);
    add(userLabel);

    userIdLabel.set_label("-");
    userIdLabel.set_selectable(true);
    userIdLabel.set_valign(Gtk::Align::ALIGN_CENTER);
    userIdLabel.set_margin_end(10);
    add(userIdLabel);

    // Reconnect:
    reconnectBtn.set_label("Reconnect");
    reconnectBtn.set_margin_end(10);
    reconnectBtn.set_margin_start(10);
    reconnectBtn.set_margin_top(10);
    reconnectBtn.set_margin_bottom(10);
    reconnectBtn.set_valign(Gtk::Align::ALIGN_CENTER);
    reconnectBtn.set_halign(Gtk::Align::ALIGN_END);
    reconnectBtn.signal_clicked().connect(sigc::mem_fun(this, &StatusBarWidget::on_reconnect_clicked));
    Glib::RefPtr<Gtk::StyleContext> reconnectBtnStyleCtx = reconnectBtn.get_style_context();
    reconnectBtnStyleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    reconnectBtnStyleCtx->add_class("coffee-button");
    this->pack_end(reconnectBtn);
}

//-----------------------------Events:-----------------------------
void StatusBarWidget::on_logout_clicked() {
    m_signal_logout_clicked.emit();
}

void StatusBarWidget::on_reconnect_clicked() {
    m_signal_reconnect_clicked.emit();
}
}  // namespace ui::widgets
