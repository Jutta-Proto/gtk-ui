#include "CoffeeSelectionWidget.hpp"
#include "jutta_bt_proto/CoffeeMaker.hpp"
#include "jutta_bt_proto/CoffeeMakerLoader.hpp"
#include "ui/utils/UiUtils.hpp"
#include <cassert>
#include <giomm/file.h>
#include <gtkmm/enums.h>
#include <sigc++/functors/mem_fun.h>
#include <spdlog/spdlog.h>

namespace ui::widgets {
CoffeeSelectionWidget::CoffeeSelectionWidget() {
    prep_widget();
}

void CoffeeSelectionWidget::prep_widget() {
    set_selection_mode(Gtk::SelectionMode::SELECTION_NONE);
    set_homogeneous(true);
    set_column_spacing(10);
    set_row_spacing(10);
    set_margin_bottom(10);
    set_margin_top(10);
    set_margin_start(10);
    set_margin_end(10);
    set_vexpand(false);
    set_valign(Gtk::Align::ALIGN_START);

    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
}

void CoffeeSelectionWidget::set_user_profile(backend::storage::UserProfile* profile) {
    this->profile = profile;
}

void CoffeeSelectionWidget::set_coffee_maker(std::shared_ptr<jutta_bt_proto::CoffeeMaker> coffeeMaker) {
    if (joeHandle) {
        this->coffeeMaker->joeChangedEventHandler.remove(*joeHandle);
        joeHandle = std::nullopt;
    }

    this->coffeeMaker = std::move(coffeeMaker);
    if (this->coffeeMaker) {
        joeHandle = this->coffeeMaker->joeChangedEventHandler.append([this](const std::shared_ptr<jutta_bt_proto::Joe>& joe) { on_joe_changed(joe); });
    }
    on_joe_changed(this->coffeeMaker ? this->coffeeMaker->get_joe() : nullptr);
}

CoffeeSelectionWidget::type_signal_edit_custom_coffee_clicked CoffeeSelectionWidget::signal_edit_custom_coffee_clicked() {
    return m_signal_edit_custom_coffee_clicked;
}

void CoffeeSelectionWidget::add_product(const jutta_bt_proto::Product& product, const Glib::RefPtr<Gtk::CssProvider>& cssProvider) {
    std::unique_ptr<CoffeeButton> btn = std::make_unique<CoffeeButton>(product, "/ui/coffee.png", cssProvider);
    products.emplace_back(std::move(btn));
    products.back()->signal_clicked_sender().connect(sigc::mem_fun(this, &CoffeeSelectionWidget::on_coffee_button_clicked));
    add(*products.back());
}

void CoffeeSelectionWidget::clear_products() {
    for (const std::unique_ptr<CoffeeButton>& btn : products) {
        remove(*btn);
    }
    products.clear();
}

//-----------------------------Events:-----------------------------
void CoffeeSelectionWidget::on_coffee_button_clicked(const jutta_bt_proto::Product& product) {
    assert(coffeeMaker);
    coffeeMaker->request_coffee(product);
    SPDLOG_INFO("Requesting product " + product.name + " (" + product.code + ")");
}

void CoffeeSelectionWidget::on_brew_custom_coffee_clicked(const jutta_bt_proto::Product& /*product*/) {
    assert(coffeeMaker);
    // bool cancel = false;
    // std::chrono::milliseconds beansTime = std::chrono::milliseconds{static_cast<int64_t>(3600.0 * profile->beansFactor)};
    // std::chrono::milliseconds waterTime = std::chrono::milliseconds{static_cast<int64_t>(40000.0 * profile->waterFactor)};
    // coffeeMaker->get_coffee_maker()->brew_custom_coffee(&cancel, beansTime, waterTime);
}

void CoffeeSelectionWidget::on_edit_custom_coffee_clicked(const jutta_bt_proto::Product& /*product*/) {
    m_signal_edit_custom_coffee_clicked.emit();
}

void CoffeeSelectionWidget::on_joe_changed(const std::shared_ptr<jutta_bt_proto::Joe>& joe) {
    clear_products();
    if (!joe) {
        return;
    }
    // Style:
    Glib::RefPtr<Gtk::CssProvider> cssProvider = get_css_provider();
    for (const jutta_bt_proto::Product& product : joe->products) {
        add_product(product, cssProvider);
    }
    show_all();
}
}  // namespace ui::widgets
