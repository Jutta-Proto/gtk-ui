#include "EditCustomCoffeeWidget.hpp"
#include "backend/storage/UserProfileStorage.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>
#include <chrono>
#include <logger/Logger.hpp>
#include <bits/stdint-intn.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/scale.h>
#include <spdlog/spdlog.h>

namespace ui::widgets {
EditCustomCoffeeWidget::EditCustomCoffeeWidget() {
    prep_widget();
}

void EditCustomCoffeeWidget::prep_widget() {
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    Glib::RefPtr<Gtk::StyleContext> styleCtx = get_style_context();
    styleCtx->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    styleCtx->add_class("overlay-frame-background");

    set_halign(Gtk::Align::ALIGN_CENTER);
    set_valign(Gtk::Align::ALIGN_CENTER);
    set_margin_bottom(10);
    set_margin_top(10);
    set_margin_left(10);
    set_margin_right(10);
    set_size_request(350, -1);

    mainBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    mainBox->set_margin_bottom(10);
    mainBox->set_margin_top(10);
    mainBox->set_margin_left(10);
    mainBox->set_margin_right(10);

    // Text:
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("Change settings here to manipulate the amount of water, beans and the temperature of your custom coffee.\nThe default value of 1.00 represets roughly a default coffee.");
    label->set_line_wrap(true);
    label->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD);
    mainBox->add(*label);

    // Scales:
    add(*mainBox);
    Gtk::Label* waterLabel = Gtk::make_managed<Gtk::Label>("Water:");
    waterLabel->set_halign(Gtk::Align::ALIGN_START);
    mainBox->add(*waterLabel);
    waterScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(1, 0.5, 2), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    waterScale->signal_button_release_event().connect(sigc::mem_fun(this, &EditCustomCoffeeWidget::on_water_scale_button_released));
    waterScale->set_digits(2);
    waterScale->set_draw_value();
    mainBox->add(*waterScale);
    Gtk::Label* beansLabel = Gtk::make_managed<Gtk::Label>("Beans:");
    beansLabel->set_margin_top(10);
    beansLabel->set_halign(Gtk::Align::ALIGN_START);
    mainBox->add(*beansLabel);
    beansScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(1, 0.5, 2), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    beansScale->signal_button_release_event().connect(sigc::mem_fun(this, &EditCustomCoffeeWidget::on_beans_scale_button_released));
    beansScale->set_digits(2);
    beansScale->set_draw_value();
    mainBox->add(*beansScale);
    Gtk::Label* tempLabel = Gtk::make_managed<Gtk::Label>("Temperature:");
    tempLabel->set_margin_top(10);
    tempLabel->set_halign(Gtk::Align::ALIGN_START);
    mainBox->add(*tempLabel);
    tempScale = Gtk::make_managed<Gtk::Scale>(Gtk::Adjustment::create(1, 0.5, 2), Gtk::Orientation::ORIENTATION_HORIZONTAL);
    tempScale->signal_button_release_event().connect(sigc::mem_fun(this, &EditCustomCoffeeWidget::on_temp_scale_button_released));
    tempScale->set_digits(2);
    tempScale->set_draw_value();
    mainBox->add(*tempScale);

    // Buttons:
    Gtk::Box* buttonsBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_HORIZONTAL);
    buttonsBox->set_margin_top(10);
    mainBox->add(*buttonsBox);
    Gtk::Button* backBtn = Gtk::make_managed<Gtk::Button>("Back");
    backBtn->set_halign(Gtk::Align::ALIGN_START);
    backBtn->signal_clicked().connect(sigc::mem_fun(this, &EditCustomCoffeeWidget::on_back_clicked));
    buttonsBox->pack_start(*backBtn);
    Gtk::Button* resetBtn = Gtk::make_managed<Gtk::Button>("Reset");
    resetBtn->set_halign(Gtk::Align::ALIGN_END);
    resetBtn->signal_clicked().connect(sigc::mem_fun(this, &EditCustomCoffeeWidget::on_reset_clicked));
    buttonsBox->pack_end(*resetBtn);
}

void EditCustomCoffeeWidget::set_user_profile(backend::storage::UserProfile* profile) {
    this->profile = profile;
    if (profile) {
        waterScale->set_value(profile->waterFactor);
        beansScale->set_value(profile->beansFactor);
        tempScale->set_value(profile->tempFactor);
    } else {
        waterScale->set_value(1);
        beansScale->set_value(1);
        tempScale->set_value(1);
    }
}

EditCustomCoffeeWidget::type_signal_clicked EditCustomCoffeeWidget::signal_back_clicked() {
    return m_signal_back_clicked;
}

EditCustomCoffeeWidget::type_signal_profile_value_changed EditCustomCoffeeWidget::signal_profile_value_changed() {
    return m_signal_profile_value_changed;
}

//-----------------------------Events:-----------------------------
bool EditCustomCoffeeWidget::on_water_scale_button_released(GdkEventButton* event) {
    if (event->button == 1 && profile) {
        double waterFactor = waterScale->get_value();
        if (profile->waterFactor != waterFactor) {
            profile->waterFactor = waterFactor;
            backend::storage::get_user_profile_storage_instance().write_profiles();
            m_signal_profile_value_changed.emit(profile);
            SPDLOG_DEBUG("Water factor updated to: {}", waterFactor);
        }
    }
    return false;
}

bool EditCustomCoffeeWidget::on_beans_scale_button_released(GdkEventButton* event) {
    if (event->button == 1) {
        double beansFactor = beansScale->get_value();
        if (profile->beansFactor != beansFactor) {
            profile->beansFactor = beansFactor;
            backend::storage::get_user_profile_storage_instance().write_profiles();
            m_signal_profile_value_changed.emit(profile);
            SPDLOG_DEBUG("Beans factor updated to: {}", beansFactor);
        }
    }
    return false;
}

bool EditCustomCoffeeWidget::on_temp_scale_button_released(GdkEventButton* event) {
    if (event->button == 1) {
        double tempFactor = tempScale->get_value();
        if (profile->tempFactor != tempFactor) {
            profile->tempFactor = tempFactor;
            backend::storage::get_user_profile_storage_instance().write_profiles();
            m_signal_profile_value_changed.emit(profile);
            SPDLOG_DEBUG("Temp factor updated to: {}", tempFactor);
        }
    }
    return false;
}

void EditCustomCoffeeWidget::on_back_clicked() {
    m_signal_back_clicked.emit();
}

void EditCustomCoffeeWidget::on_reset_clicked() {
    assert(profile);
    profile->waterFactor = 1;
    profile->beansFactor = 1;
    profile->tempFactor = 1;
    set_user_profile(profile);
    backend::storage::get_user_profile_storage_instance().write_profiles();
    m_signal_profile_value_changed.emit(profile);
    SPDLOG_DEBUG("Profile factors reset.");
}

}  // namespace ui::widgets
