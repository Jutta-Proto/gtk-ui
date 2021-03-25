#pragma once

#include <gtkmm.h>
#include <gtkmm/flowbox.h>

namespace ui::widgets {
class CoffeeSelectionWidget : public Gtk::FlowBox {
 public:
    CoffeeSelectionWidget();

 private:
    void prep_widget();
    static Gtk::Button& generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets
